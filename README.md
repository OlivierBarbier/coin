# coin

Open 4 terminals: term1, term2, term3 and term4.

In term1

`cd <repo>`

`git clone https://github.com/OlivierBarbier/coin.git`

`cd coin`

`make all`

In term2

`cd <repo>/coin`

`./bin/coin_ledger_serve`

In term3, term4

`cd <repo>/coin/scripts`

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
