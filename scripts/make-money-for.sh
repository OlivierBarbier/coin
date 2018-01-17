# ./virement.sh alice bob 1

from="bank"
to=$1
from_keypair="../keys/$from-keypair.bin"
from_pubkey="../keys/$from-pubkey.bin"
to_pubkey="../keys/$to-pubkey.bin"
amount=$2

echo "=== Ordre de virement ==="
echo "Emetteur: $from"
echo "Destinataire: $to"
echo "Montant: $amount"
echo "=== Debug ==="
echo "$from keypair : $from_keypair"
echo "$from pubkey : $from_pubkey"
echo "$to pubkey : $from_keypair"


../bin/coin_transfer_create BANK $to $from_pubkey $to_pubkey $from-transfer.bin $amount
../bin/coin_message_create $from-transfer.bin $from_keypair $from-message.bin
../bin/coin_message_perform_mining ../ledger.bin $from-message.bin $from-message-mined.bin

nc -w 0 -u 0.0.0.0 7777 < $from-message-mined.bin
echo "Transmission: ok"
