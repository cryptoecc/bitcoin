// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <ldpc/LDPC.h>

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        /*In the mainnetmode, fPoWAllowMinDiffcultyBlocks is set to be negative.
         *Thus, we don't care the following routines
         */
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;
 

    printf("CalculateNextWorkRequired: Height (before): %s\n", pindexLast->nHeight);
    printf("curt_level = %d\n",GetLevelfromnBits(pindexLast->nBits));

    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;


    printf("next_level = %d\n",GetLevelfromnBits(bnNew.GetCompact()));

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}

#if LDPC_POW
bool CheckProofOfWork(CBlockHeader block, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    bool result = false;

    arith_uint256 bnTarget;

    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return result;

    int level = GetLevelfromnBits(block.nBits);
    LDPC *ldpc = new LDPC;
    ldpc->set_difficulty(level);
    ldpc->initialization();
    ldpc->generate_seeds(UintToArith256(block.hashPrevBlock).GetLow64());
    ldpc->generate_H();
    ldpc->generate_Q();
    ldpc->generate_hv((unsigned char*)block.GetHash().ToString().c_str());
    ldpc->decoding();
    if (ldpc->decision())
        result = true;

    delete ldpc;
    return result;
}
int GetLevelfromnBits(uint32_t nBits)
{
    int level = 1;
    double dDiff = 0.0, log_dDiff = 0.0;
    dDiff = GetDifficulty(nBits);
    log_dDiff = log2(dDiff);

    /*
     *   0<=log_dDiff<0.3 => 1
     * 0.3<=log_dDiff<0.6 => 2 
     * 0.6<=log_dDiff<0.9 => 3
     * Thus the level is floor(log_dDiff/0.3))+1
     */
    level = floor(log_dDiff/0.3) + 1;

    if ( level >= 380 )
	    level = 380;
    if (level <= 1 )
	    level = 1;

    return level;
}

double GetDifficulty(uint32_t nBits)
{
    int nShift = (nBits >> 24) & 0xff;
    double dDiff =
        (double)0x0000ffff / (double)(nBits & 0x00ffffff);

    while (nShift < 29)
    {
        dDiff *= 256.0;
        nShift++;
    }
    while (nShift > 29)
    {
        dDiff /= 256.0;
        nShift--;
    }
    return dDiff;
}
#endif
