from=$1
../bin/coin_ledger_compute_balances ../ledger.bin | grep $from | uniq | grep -o "[0-9]" | xargs echo "vBTC"