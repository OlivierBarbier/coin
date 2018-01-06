# coin

Open 4 terminals: term1, term2, term3 and term4.

In term1, term2, term3 and term 4

`cd <repo>`

In term1

`git clone`
`make all`

In term3, term4

`cd scripts`

In term2

`./bin/coin_ledger_serve`

In term3

`./balance alice`

In term4

`./balance bob`

term3

`./virement alice bob 1`

wait 1 second

`./balance alice`

term4

`./balance bob
