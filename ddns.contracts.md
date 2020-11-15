<h1 class="contract">add</h1>
---
spec-version: 0.0.1
title: Add
summary: This action will either add or modify a dDNS record for a particular domain. If an entry exists with the same rname parameter, the record is updated with the domain, rname, ttl, rclass, rtype and rdata parameters. If a record does not exist, a new record is created. The data is stored in a multi index table. The ram costs are paid by the smart contract.
icon:

<h1 class="contract">erase</h1>
---
spec-version: 0.0.1
title: Erase
summary: This action will remove a dDNS record for a particular domain if an entry in the multi index table exists with the specified rname.
icon: