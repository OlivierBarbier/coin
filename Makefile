bin/coin_ledger_create: src/coin_ledger_create.c
	gcc src/coin_ledger_create.c -o bin/coin_ledger_create

bin/coin_ledger_dump: src/coin_ledger_dump.c
	gcc src/coin_ledger_dump.c -o bin/coin_ledger_dump

bin/coin_ledger_compute_balances: src/coin_ledger_compute_balances.c
	gcc src/coin_ledger_compute_balances.c -o bin/coin_ledger_compute_balances

bin/coin_keypair_generate: src/coin_keypair_generate.c obj/uECC.o
	gcc src/coin_keypair_generate.c obj/uECC.o -o bin/coin_keypair_generate

bin/coin_transfer_create: src/coin_transfer_create.c
	gcc src/coin_transfer_create.c -o bin/coin_transfer_create

bin/coin_message_create: src/coin_message_create.c obj/uECC.o obj/sha256.o
	gcc src/coin_message_create.c obj/uECC.o obj/sha256.o -o bin/coin_message_create

bin/coin_message_consume: src/coin_message_consume.c obj/uECC.o obj/sha256.o
	gcc src/coin_message_consume.c obj/uECC.o obj/sha256.o -o bin/coin_message_consume

bin/coin_message_perform_mining: src/coin_message_perform_mining.c obj/uECC.o obj/sha256.o
	gcc src/coin_message_perform_mining.c obj/uECC.o obj/sha256.o -o bin/coin_message_perform_mining

bin/coin_ledger_serve: src/coin_ledger_serve.c
	gcc src/coin_ledger_serve.c -o bin/coin_ledger_serve

webserver: src/webserver.c obj/coin_ledger_compute_balances.o
	gcc src/webserver.c obj/coin_ledger_compute_balances.o -lmicrohttpd -o bin/webserver

ledger:
	./bin/coin_ledger_create ledger.bin

bank:
	./bin/coin_keypair_generate ./keys/bank-keypair.bin ./keys/bank-pubkey.bin

ali:
	./bin/coin_keypair_generate ./keys/ali-keypair.bin ./keys/ali-pubkey.bin

bob:
	./bin/coin_keypair_generate ./keys/bob-keypair.bin ./keys/bob-pubkey.bin

bank-transfer:
	./bin/coin_transfer_create BANK ali ./keys/bank-pubkey.bin ./keys/ali-pubkey.bin bank-transfer.bin 7

bank-message:
	./bin/coin_message_create bank-transfer.bin ./keys/bank-keypair.bin bank-message.bin

bank-message-mining:
	./bin/coin_message_perform_mining ledger.bin bank-message.bin bank-message-mined.bin

bank-consume:
	./bin/coin_message_consume ledger.bin bank-message-mined.bin

transfer:
	./bin/coin_transfer_create ali bob ./keys/ali-pubkey.bin ./keys/bob-pubkey.bin transfer.bin 3

message:
	./bin/coin_message_create transfer.bin ./keys/ali-keypair.bin message.bin

consume:
	./bin/coin_message_consume ledger.bin message.bin

dump:
	./bin/coin_ledger_dump ledger.bin

balance:
	./bin/coin_ledger_compute_balances ledger.bin

serve:
	./bin/coin_ledger_serve

ipfs-start:
	./lib/ipfs daemon --enable-pubsub-experiment

ipfs-fifo:
	mkfifo message.fifo

ipfs-message-forward:
	./lib/ipfs pubsub sub TX > message.fifo

ipfs-message-publish:
	./lib/ipfs pubsub pub TX < message.bin

ipfs-message-consume:
	./bin/coin_message_consume ledger.bin message.fifo

clean:
	rm ./*.bin
	rm ./bin/*
	rm ./keys/*.bin
	rm scripts/*.bin
	rm message.fifo

bin: bin/coin_ledger_create \
	bin/coin_ledger_dump \
	bin/coin_ledger_compute_balances \
	bin/coin_keypair_generate \
	bin/coin_transfer_create \
	bin/coin_message_perform_mining \
	bin/coin_message_create \
	bin/coin_message_consume \
	bin/coin_ledger_serve

all: bin \
	ledger bank ali bob\
	bank-transfer bank-message bank-message-mining bank-consume \
	transfer message consume \
	dump balance
