# coin

Open 4 terminals: term1, term2, term3 and term4.

In term1

`cd /tmp`

`git clone https://github.com/OlivierBarbier/coin.git`

`cd coin`

`make all`

In term2

`cd /tmp/coin`

`./bin/coin_ledger_serve`

In term3, term4

`cd /tmp/coin/scripts`

In term3

`./balance.sh alice /* vBTC 4 */`

In term4

`./balance.sh bob /* vBTC 3 */`

term3

`./virement.sh alice bob 1`

wait 1 second

`./balance.sh alice /* vBTC 3 */`

term4

`./balance.sh bob /* vBTC 4 */`
