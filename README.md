# coin

## Basic Installation

Oh My Zsh is installed by running one of the following commands in your terminal. You can install this via the command-line with either `curl` or `wget`.

### via curl

```shell
sh -c "$(curl -fsSL https://raw.githubusercontent.com/OlivierBarbier/coin/master/coin-install.sh)"
```

## 
Open 2 terminals: term1 and term2.

In term1

`cd /tmp`

`git clone https://github.com/OlivierBarbier/coin.git`

`cd coin`

`make all`

`./bin/coin_ledger_serve`

In term2

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
