/src/db: ./src/db.c
		gcc ./src/db.c -o ./src/db

run: /src/db
		./src/db mydb.db

clear: /src/db
		rm mydb.db

test: /src/db
		make
		bundle exec rspec