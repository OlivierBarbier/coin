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

ledger:
	./bin/coin_ledger_create ledger.bin

bank:
	./bin/coin_keypair_generate ./keys/bank-keypair.bin ./keys/bank-pubkey.bin

alice:
	./bin/coin_keypair_generate ./keys/alice-keypair.bin ./keys/alice-pubkey.bin

bob:
	./bin/coin_keypair_generate ./keys/bob-keypair.bin ./keys/bob-pubkey.bin

bank-transfer:
	./bin/coin_transfer_create BANK alice ./keys/bank-pubkey.bin ./keys/alice-pubkey.bin bank-transfer.bin 7

bank-message:
	./bin/coin_message_create bank-transfer.bin ./keys/bank-keypair.bin bank-message.bin

bank-message-mining:
	./bin/coin_message_perform_mining ledger.bin bank-message.bin bank-message-mined.bin

bank-consume:
	./bin/coin_message_consume ledger.bin bank-message-mined.bin

transfer:
	./bin/coin_transfer_create alice bob ./keys/alice-pubkey.bin ./keys/bob-pubkey.bin transfer.bin 3

message:
	./bin/coin_message_create transfer.bin ./keys/alice-keypair.bin message.bin

consume:
	./bin/coin_message_consume ledger.bin message.bin

dump:
	./bin/coin_ledger_dump ledger.bin

balance:
	./bin/coin_ledger_compute_balances ledger.bin

clean:
	rm ./*.bin
	rm ./bin/*
	rm ./keys/*.bin
	rm scripts/*.bin

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
	ledger bank alice bob\
	bank-transfer bank-message bank-message-mining bank-consume \
	transfer message consume \
	dump balance
