# coin

## Basic Installation

### via curl

```shell
sh -c "$(curl -fsSL https://raw.githubusercontent.com/OlivierBarbier/coin/master/coin-install.sh)"
```

### manually 
`cd /tmp`

`git clone https://github.com/OlivierBarbier/coin.git`

`cd coin`

`make all`

`./bin/coin_ledger_serve`

## Usage

`cd /tmp/coin/scripts`

`./balance.sh alice`

-> vBTC 4

`./balance.sh bob`

-> vBTC 3

`./virement.sh alice bob 4`

wait 1 second

`./balance.sh alice`

-> vBTC 0

`./balance.sh bob`

-> vBTC 7
