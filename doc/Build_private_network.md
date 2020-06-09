# Build private network and test (BitcoinECC)

Writer : Hyunjun Jung(정현준)

Email : junghj85@gist.ac.kr / junghj85@gmail.com

Github for this example : https://github.com/cryptoecc/bitcoin_ECC

For more information : [INFONET](https://infonet.gist.ac.kr/)

Today we will build our own private network and test it is working well. However, we will use only 1 node for today. Let's try multi-node later.

## Environment

- Ubuntu 18.04.2 LTS or later (http://old-releases.ubuntu.com/releases/18.04.2/)
- Git :  https://github.com/cryptoecc/bitcoin_ECC (ver0.1.2)



Step1 : Install of build tool

```
sudo apt-get install build-essential libtool autotools-dev automake pkg-config bsdmainutils python3
```

```
sudo apt-get install libqrencode-dev autoconf openssl libssl-dev libevent-dev libminiupnpc-dev
```



Step2 : Install of boost labrary

```
sudo apt-get install libboost-all-dev
```



Step3 : Install of berkeley DB

```
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install libdb4.8-dev libdb4.8++-dev
```



Step4 : Install of qt-wallet

```
sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev
```



Step5 : Install of git

```
sudo apt-get install git
```



Step6 : ECCPoW blockchain code download and version change

```
git clone https://github.com/cryptoecc/bitcoin_ECC.git
cd bitcoin_ECC
git checkout ecc-0.1.2
```



Step7 : Source code build

```
cd bitcoin_ECC
./autogen.sh
./configure
make
sudo make install
```



Step8 : Execute bitcoin core

```
bitcoind -txindex -printtoconsole -rpcallowip=0.0.0.0/0 -rpcbind=0.0.0.0
```

or

```
bitcoind -txindex -daemon
```



## ECCPoW Blockchain Parameters

The ECCPoW block chain can build Mainnet, Testnet, and Regest networks. You can set up private network using the CMainParams, CTestNetParams, CRegTestParams class of "chainparams.cpp". We will test the private network using mainnet.



chainparams.cpp source code

```
class CMainParams : public CChainParams {
public:
  CMainParams() {
	 (.....)
  consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffff
ffffffffffffffff");
  consensus.nPowTargetTimespan = 60 * 60;
  consensus.nPowTargetSpacing = 1 * 60;
  consensus.fPowAllowMinDifficultyBlocks = true;
  consensus.fPowNoRetargeting = true;
    (.....)
  nDefaultPort = 9777; 
  int init_level = 10;
  genesis = CreateGenesisBlock(1558627231, 399, init_level, 1, 50 *     COIN); 
  assert(consensus.hashGenesisBlock == uint256S("b77abb03a0a8a4f23a73
80bf655af8312c4769c64fcbf335a08d598b13368f22")); 
  assert(genesis.hashMerkleRoot == uint256S("15d2f927fe3eafe88ce0b4ccf2
67727ed306295051339a16e0b95067e65bead8"));
    (.....)
  vSeeds.emplace_back("...");
    (.....)
  m_fallback_fee_enabled = false;
  }
};
```

| Parameters                   | description                                                  |
| ---------------------------- | ------------------------------------------------------------ |
| powLimit                     | Minimum difficulty setting                                   |
| nPowTargetTimespan           | Difficulty level change cycle (currently 60 minutes)         |
| nPowTargetSpacing            | Block generation cycle (currently 1 minute)                  |
| fPowAllowMinDifficultyBlocks | Permission below minimum difficulty level                    |
| fPowNoRetargeting            | Permission to change difficulty level                        |
| nDefaultPort                 | ECCPoW Blockchain port number (currently port 9777)          |
| init_level                   | ECCPoW difficulty level (currently difficulty level 10)      |
| CreateGenesisBlock           | Linux time, Nonce, nBit, nVersion, compensation amount entered in order to generation the Genesis block |
| vSeeds.emplace_back          | Connection seed address                                      |
| m_fallback_fee_enabled       | Permission of coin transfer fee setting                      |





## Test private network

***Now we can run our private network!***

open terminal in `/bitcoin_ECC` and follow it!

```
jun@ubuntu:~/bitcoin_ECC$ bitcoind -txindex -daemon

mainnet with level = 1
set is constructed from 10 to 22 with step 2
n : 32	 wc : 3	 wr : 4
mainnet n: 399 Hash: 0ccc07b781737bf3c901307dd92ed0548215b745b38daee5fdbb4f042dc4885c

testnet with level = 40
set is constructed from 30 to 30 with step 2
n : 60	 wc : 3	 wr : 4
testnet n: 47471 Hash: 83432832323855c30a104d61467ecb339ffa988cb8abe420c67acb95667bdd42

regtest with level = 30
set is constructed from 18 to 42 with step 2
n : 60	 wc : 3	 wr : 4
regtest n: 746 Hash: db20aafd0c626db7cf83b4b3d9ba0a024a862fdff203074e06aaa64e31619461

mainnet with level = 1
set is constructed from 10 to 22 with step 2
n : 32	 wc : 3	 wr : 4
mainnet n: 399 Hash: 0ccc07b781737bf3c901307dd92ed0548215b745b38daee5fdbb4f042dc4885c
Bitcoin server starting
```



Now let's test our private network

```
jun@ubuntu:~/bitcoin_ECC$ bitcoin-cli getblockcount
0
```

`bitcoin-cli getblockcount` check the number of blocks. We just generate this network. Therefore there is 0 block. 



Let's generate account

```
jun@ubuntu:~/bitcoin_ECC$ bitcoin-cli getnewaddress
39naMhHQgXwm7CVdEaL4PGUkp4fUdHX14M
```

We just generated the address of `39naMhHQgXwm7CVdEaL4PGUkp4fUdHX14M`. 



Let's generate block

```
jun@ubuntu:~/bitcoin_ECC$ bitcoin-cli generatetoaddress 10 9naMhHQgXwm7CVdEaL4PGUkp4fUdHX14M
[
  "fa2549dbcbecfbaff696abd19e086b82e7288cc3c726b6fc4cfc3754a4200a31",
  "09df0e57e1a4f84c594a5553dd2c3cc01c380ba92bf37268a84fbad62ed79693",
  "abc2bce6e224b68da443b505d779cf91715119847d457068c47fa0529dbf71a4",
  "951ae98c7193e11f7469245ec2f18a20103e8e26afe3d7eef10e7a22d15ed2dd",
  "c55a5294d3f8b0c3259cf3842ad0ad9bbb3309523938919dbd59cd1cb14b530a",
  "b2d7b23c8c3ba403b05f6111f49c65212eba0ca61378e15612c04269e3177a91",
  "c4ea7cfc8cafe7e9e8989a5c5808d3e01e83794ebd56278ea9013571e480791d",
  "ac8eb56da73ee9bce603cdda83b1a89903e6cd6828ea5f9c6ab0c8572a780f4b",
  "bd1f46d19d6db8c35536594e6a39339c15ef0185bd244afe7c0af022e6ebf29e",
  "5d4cc21450bcb8df125b8a5301e80f2bc35151ffd1d47a65999238073733507e"
]
```

Let's check blockchain information

```
jun@ubuntu:~/bitcoin_ECC$ bitcoin-cli getblockchaininfo
{
  "chain": "main",
  "blocks": 10,
  "headers": 10,
  "bestblockhash": "5d4cc21450bcb8df125b8a5301e80f2bc35151ffd1d47a65999238073733507e",
  "difficulty": 4.523059468369196e+74,
  "mediantime": 1590478786,
  "verificationprogress": 1,
  "initialblockdownload": false,
  "chainwork": "00000000000000000000000000000000000000000000000000000000000573f8",
  "size_on_disk": 3285,
  "pruned": false,
  "softforks": [
    {
      "id": "bip34",
      "version": 2,
      "reject": {
        "status": false
      }
    },
    {
      "id": "bip66",
      "version": 3,
      "reject": {
        "status": false
      }
    },
    {
      "id": "bip65",
      "version": 4,
      "reject": {
        "status": false
      }
    }
  ],
  "bip9_softforks": {
    "csv": {
      "status": "defined",
      "startTime": 1462060800,
      "timeout": 1493596800,
      "since": 0
    },
    "segwit": {
      "status": "active",
      "startTime": -1,
      "timeout": 9223372036854775807,
      "since": 0
    }
  },
  "warnings": ""
}

```

Let's check balance

```
jun@ubuntu:~/bitcoin_ECC$ bitcoin-cli getbalance
450.00000000
```

jun@ubuntu:~/bitcoin_ECC$bitcoin-cli gettransaction f2fc88a8bc534cfb7e6bc8e7c7d944f440a6612a8b7d34c166ca76765e168c56
{
"amount":0.00000000,
"fee":-0.00050000,
"confirmations":1,
"blockhash":"f1d741990690527423d076fc77e8f26d26dc12b90251f912fcb3332253bdb615",
"blockindex":1,
"blocktime":1591687898,
"txid":"f2fc88a8bc534cfb7e6bc8e7c7d944f440a6612a8b7d34c166ca76765e168c56",
"walletconflicts":[
],
"time":1591687164,
"timereceived":1591687164,
"bip125-replaceable":"no",
"details":[
{
"address":"345g89pujpmSSFuCWeizqbvVe92pRWZarB",
"category":"send",
"amount":-0.02500000,
"label":"",
"vout":0,
"fee":-0.00050000,
"abandoned":false
},
{
"address":"39naMhHQgXwm7CVdEaL4PGUkp4fUdHX14M",
"category":"send",
"amount":-49.97450000,
"label":"",
"vout":1,
"fee":-0.00050000,
"abandoned":false
},
{
"address":"345g89pujpmSSFuCWeizqbvVe92pRWZarB",
"category":"receive",
"amount":0.02500000,
"label":"",
"vout":0
},
{
"address":"39naMhHQgXwm7CVdEaL4PGUkp4fUdHX14M",
"category":"receive",
"amount":49.97450000,
"label":"",
"vout":1
}
],
"hex":"0200000000010144c4df3d038eb8c5f247e452abe2bf729a00fe13040a34beb2e0c0072895cdc10000000017160014869c8d0d64aa4aba5a8336821384b3c8e8631026ffffffff02a02526000000000017a9141a394a65ad9ebfc907bac968a4e86cbb164c9c85871009df290100000017a91458ce290b131d50345f67f0c330d900be55c5e8f58702473044022075b8645993714ba61c63a177437c3146145a9daebf25f93324eadf767e0277cd02202648a4817686bdddb57944c80e9a0a3a4daeb8defd59d4fc1d118fd814392909012103290574714e059efbb6248b7058901639932d4d32a6f3682cf21f0dd74bf4bf4100000000"
}
