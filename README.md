# coin

Open 4 terminals (term1, term2, term3 and term4).

term1, term2, term3 and term 4
cd <repo>

term1
git clone
make all

term3, term4
cd scripts

term2
./bin/coin_ledger_serve

term3
./balance alice

term4
./balance bob

term3
./virement alice bob 1
wait 1 second
./balance alice

term4
./balance bob
