#dDNS Contract
This is the smart contract related to the official implementation of [dWeb's dDNS](https://github.com/peepsx/ddns-whitepaper) `Root System`.

## About The dDNS Root System
The Root System stores `dWeb Discovery Keys` related to a decentralized domain's `NameDrive (ND)` record(s), similar to how the Internet's dDNS system stores IP addresses related to `Name Server (NS)` record(s) for a centralized domain. The Root System only adds the ND record type, other record types for a dTLD belong within a domain's database within its authoratative NameDrives. [dWeb](https://dwebx.org) browsers like [dBrowser](http://dbrowser.com) first performs an ND record lookup at the dDNS `Root System` in order to download an ND and perform record queries on a particular domain's database that is distributed within the ND itself.

```
User(<->)dBrowser(<->)dDNS Root Lookup(<->)Download ND(<->)Query ND Domain Database For Record
```
## Actions
### `regnd`
Allows `domain` owner to create or modify `record_name` for a NameDrive (ND) and its corresponding `dweb_key`. 

#### `regnd` Parameters
- `domain` - The domain the ND record is associated with (secondary indice).
- `record_name` - the name of the ND record ie. `nd1`. 
- `dweb_key` - The ND's dWeb discovery key.

**NOTE:** A `record_num` is automatically generated as the `primary_key()` for each `regnd`-based addition to the `ndrecords` table. The `record_num` consists of hte elapsed seconds since epoch (At the time the action is ran) plus a random number between 1 and 8888888. This should insure that no two record numbers are the same.

The contract uses an iterator to first determine if the `record_name` being added already exists. If so, the information passed to `regnd` will modify the matching record. In turn, a new `record_num` is generated for the modified record (different from its original `record_num`)

### `removend` 
This action removes a NameDrive record for a particular domain from the Root System.

#### `removend` Parameters
- `domain` - The domain the ND record is being removed from. (secondary indice)
- `record_num` - The `record_num` of the ND record being removed (primary index)

## Compile This Contract
```
arisen-cpp -o ddns.wasm ddns.cpp --abigen
```

## Deploy Contract On [ARISEN](https://arisen.network)
```
arisecli set contract ddns CONTRACTS_DIR/ddns
```

**NOTE:** This will not actually work since this contract already exists on ARISEN under the name `ddns`. You would have to change the contract name within the ddns.hpp file and deploy to the network under that new name.

## Adding An ND Record
### Via AriseCLI
```
arisecli push action ddns regnd '["peeps.dcom", "nd1", "<dweb-key>"]' -p peeps.dcom@active
```

### Via JavaScript / Node.JS
```
const arisenjs = require('arisenjs');
const fetch = require('node-fetch');
const { TextDecoder, TextEncoder } = require('text-encoding');

// Signature Provider
const defaultPrivateKey = "<key>";
const signatureProvider = new arisenjs.SignatureProvider([defaultPrivateKey])

// Open RPC connection to ARISEN
const rpc = new arisenjs.Rpc.JsonRpc('https://greatchains.arisennodes.io', { fetch });

// Push `regnd` Action
(async () => {
  const result = await api.transact ({
    actions: [{
      account: 'ddns',
      name: 'regnd',
      authorization: [{
        actor: 'peeps.dcom',
        permission: 'active',
      }],
      data: {
        domain: 'peeps.dcom',
        record_name: 'nd1',
        dweb_key: '<dweb-key-for-record>',
      },
    }]
  }, {
    blocksBehind: 3,
    expireSeconds: 30,
  });
 console.dir(result);
)}();
```

## Lookup Record With AriseCLI
```shell
arisecli get table ddns ddns rdatadb --upper peeps.dcom \
--key-type i64 \
--index 1
```

### Response (JSON)
```
{
  "rows": [{
    "domain": "peeps.dcom",
    "record_name": "nd1",
    "dweb_key": "aa4f36bd0b2a7a8bf68aa0bdd0b93997fd8ff053f4a3e816cb629210aa17737"
  }
 ],
  "more": false,
  "next_key": ""
}
```

## Ricardian Contract & Clauses
- [ddns Ricardian Contract](ddns.contracts.md)
- [ddns Ricardian Clauses](ddns.clauses.md)

## LICENSE
[MIT](LICENSE.md)

## Copyright
Copyright 2020 PeepsLabs. All rights reserved.