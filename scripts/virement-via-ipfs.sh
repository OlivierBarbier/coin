# ./virement.sh ali bob 1

from=$1
to=$2
from_keypair="../keys/$from-keypair.bin"
from_pubkey="../keys/$from-pubkey.bin"
to_pubkey="../keys/$to-pubkey.bin"
amount=$3

echo "=== Ordre de virement ==="
echo "Emetteur: $from"
echo "Destinataire: $to"
echo "Montant: $amount"
echo "=== Debug ==="
echo "$from keypair : $from_keypair"
echo "$from pubkey : $from_pubkey"
echo "$to pubkey : $from_keypair"

../bin/coin_ledger_compute_balances ../ledger.bin | grep $from | uniq | grep -o "[0-9]" | xargs echo "$from balance:"

../bin/coin_transfer_create $from $to $from_pubkey $to_pubkey transfer.bin $amount
../bin/coin_message_create transfer.bin $from_keypair message.bin

../lib/ipfs pubsub pub TX < message.bin

echo "Transmission via ipfs : ok"
