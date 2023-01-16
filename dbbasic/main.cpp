#include <assert.h>
#include <iostream>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "Utils.h"
#include "..\..\sqlite\sqlite3.h"

using namespace sf;
using namespace std;

//Callback method for the SQL engine that displays all the returned fields and their values
//NumF - Number of fields that the query result fetches
//arrVals - An array of values represented as text
//arrNames - An array of names of the fetched fields
//Both above arrays are in the same order
static int callback(void* pNotUsed, int numF, char** arrVals, char** arrNames) {
	stringstream ss;
	int i;
	for (i = 0; i < numF; i++) {
		ss << arrNames[i] << " = " << (arrVals[i] ? arrVals[i] : "NULL") << "\n";
	}
	DebugPrint(ss.str().c_str(), "");
	return 0;
}

//Callback function for the SQL engine
//counts how many records are returned and sums the totial in gNumRecords
//Note - Make sure you reset gNumRecirds to ZERO before using this callback
int gNumRecords = 0;
static int callbackRecords(void* p, int numF, char** arv, char** arn) {
	gNumRecords++;
	return 0;
}

//Fromat: RunSQL("SQL COMMAND LINE", Database Variable, [callback] OR [nullptr], "Success debug message", "Failed Debug message")
//callback for receiving a query, nullptr for runnin an update

//first lot of text can be inputed as a variable for long lines such as inserting data into a table or creating a new table

//Query Example: RunSQL("SELECT * FROM PLAYERS WHERE USERNAME='player_name'", db, callback, "");
//Update Example: RunSQL("UPDATE PLAYERS SET LEVEL = 15 WHERE USERNAME='Player_name'", db, nullptr, "Player level saved as 15", "Could not save player's level as 15)
//Delete Example: RunSQL("DELETE FROM PLAYERS WHERE NAME = 'Player_Name'", db, nullptr, "Player_Name has been wiped", "Could not wipe Player_Name")
void RunSQL(const string& sql,
	sqlite3* db,
	int (*pCallback)(void*, int, char**, char**) = nullptr,
	const string& okPrompt = "RunSQL Successful", //Ok prompt with default response
	const string& errorPrompt = "RunSQL Failed") //Error prompt with default response
{
	//checks to make sure sql is not empty and the database exists
	assert(!sql.empty() && db);
	char* zErrMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(), pCallback, 0, &zErrMsg);
	//Checks whether or not it was successful. Returns an error if failed
	if (rc != SQLITE_OK)
	{
		DebugPrint("SQL Error: " + errorPrompt, zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		DebugPrint("OK: " + okPrompt);
	}
}


int main()
{
	// Create the main window
	RenderWindow window(VideoMode(1200, 800), "database");
	sf::Font font;
	if (!font.loadFromFile("data/fonts/comic.ttf"))
		assert(false);

	DebugPrint("Database starter app");

	//Load and open the database file
	sqlite3* db;
	int rc = sqlite3_open("data/test.db", &db);
	if (rc)
	{
		DebugPrint("Can't open database: ", sqlite3_errmsg(db));
		assert(false);
	}


	//Check if the player table is already in the database. Does not create if it is already there
	gNumRecords = 0;
	char* zErrMsg = nullptr;
	const string sql5 = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'PLAYERS'";
	rc = sqlite3_exec(db, sql5.c_str(), callbackRecords, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		DebugPrint("SQL error: ", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else if (gNumRecords == 0) {
		//Create a playerdata table
		const string sql = "CREATE TABLE PLAYERS("  \
			"ID INTEGER PRIMARY KEY     NOT NULL," \
			"USERNAME           TEXT    NOT NULL," \
			"LEVEL            INTEGER     NOT NULL," \
			"CLASS           TEXT     NOT NULL);";
		//rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
		//if (rc != SQLITE_OK) {
		//	DebugPrint("SQL player create error: ", zErrMsg);
		//	sqlite3_free(zErrMsg);
		//}
		//else {
		//	DebugPrint("Table created successfully");
		//}
		RunSQL(sql, db, nullptr);
		//Notes above are what this one line replaces
	}


	//Inserts data into the table
	const string sql2 = "INSERT or IGNORE INTO PLAYERS (ID,USERNAME,LEVEL,CLASS) "\
		"VALUES (1, 'Player_name', 15, 'Tank');";
	//rc = sqlite3_exec(db, sql2.c_str(), nullptr, 0, &zErrMsg);
	//if (rc != SQLITE_OK) {
	//	DebugPrint("SQL player insert error: ", zErrMsg);
	//	sqlite3_free(zErrMsg);
	//}
	//else {
	//	DebugPrint("Data inserted successfully");
	//}
	RunSQL(sql2, db, nullptr);
	//Notes above are what this one line replaces

	//Creates enemy data table if it does not currently exist in the database
	gNumRecords = 0;
	char* enemyErrMsg = nullptr;
	const string sql4 = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'ENEMIES'";
	rc = sqlite3_exec(db, sql4.c_str(), callbackRecords, 0, &enemyErrMsg);
	if (rc != SQLITE_OK) {
		DebugPrint("SQL error: ", enemyErrMsg);
		sqlite3_free(enemyErrMsg);
	}
	else if (gNumRecords == 0) {
		//Create a enemydata table
		
		const string enemysql = "CREATE TABLE ENEMIES("  \
			"NAME           TEXT    NOT NULL," \
			"LEVEL            INTEGER     NOT NULL," \
			"HEALTH           INTEGER     NOT NULL," \
			"DAMAGE		INTEGER		NOT NULL);";
		//rc = sqlite3_exec(db, enemysql.c_str(), nullptr, 0, &enemyErrMsg);
		//if (rc != SQLITE_OK) {
		//	DebugPrint("SQL error: ", enemyErrMsg);
		//	sqlite3_free(enemyErrMsg);
		//}
		//else {
		//	DebugPrint("Table created successfully");
		//}
		RunSQL(enemysql, db, nullptr);
		//Notes above are what this one line replaces
	}


	//Inserts data into the table
	const string enemysql2 = "INSERT or IGNORE INTO ENEMIES (NAME,LEVEL,HEALTH,DAMAGE) "\
		"VALUES ('Goblin', 5, 20, 5);";
	//rc = sqlite3_exec(db, enemysql2.c_str(), nullptr, 0, &enemyErrMsg);
	//if (rc != SQLITE_OK) {
	//	DebugPrint("SQL error: ", enemyErrMsg);
	//	sqlite3_free(enemyErrMsg);
	//}
	//else {
	//	DebugPrint("Data inserted successfully");
	//}
	RunSQL(enemysql2, db, nullptr);
	//Notes above are what this one line replaces


//	const string sql3 = "SELECT * from PLAYERS";
//	rc = sqlite3_exec(db, sql3.c_str(), callback, 0, &zErrMsg);
//	if (rc != SQLITE_OK) {
//		DebugPrint("SQL error: ", zErrMsg);
//		sqlite3_free(zErrMsg);
//	}
//	else {
//		DebugPrint("SQL select complete OK", "");
//	}
RunSQL("SELECT * from PLAYERS", db, callback, "PlayerData");
//Notes above are what this one line replaces

	Clock clock;
	// Start the game loop 
	while (window.isOpen())
	{
		bool fire = false;
		// Process events
		Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == Event::Closed) 
				window.close();
		} 

		// Clear screen
		window.clear();

		float elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Text txt("Database starter app", font, 30);
		window.draw(txt);


		// Update the window
		window.display();
	}

	sqlite3_close(db);
	db = nullptr;

	return EXIT_SUCCESS;
}
