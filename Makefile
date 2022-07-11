db: ./src/db.c
		gcc ./src/db.c -o db

run: db
		./src/db mydb.db

clear: db
		rm mydb.db

test: db
		make
		bundle exec rspec