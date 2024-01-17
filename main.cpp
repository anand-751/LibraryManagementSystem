#include <iostream>
#include <limits>
#include <sqlite3.h>
#include <iostream>
#include <limits>
#include <sqlite3.h>

class LibraryManager {
private:
    sqlite3 *db;

public:
    LibraryManager() : db(nullptr) {
        int result = sqlite3_open("library.db", &db);
        if (result != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            exit(1);
        }

        createTables();
    }

    ~LibraryManager() {
        if (db != nullptr) {
            sqlite3_close(db);
        }
    }

    void executeQuery(const std::string& query) {
        int result = sqlite3_exec(db, query.c_str(), 0, 0, 0);
        if (result != SQLITE_OK) {
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            exit(1);
        }
    }


    void createTables() {
        std::string createAuthorsTableQuery = "CREATE TABLE IF NOT EXISTS Authors (author_id INTEGER PRIMARY KEY, name TEXT)";
        std::string createBooksTableQuery = "CREATE TABLE IF NOT EXISTS Books (book_id INTEGER PRIMARY KEY, title TEXT, author_id INTEGER, edition INTEGER, floor INTEGER NOT NULL, section TEXT NOT NULL, isbn TEXT NOT NULL)";
        std::string createUsersTableQuery = "CREATE TABLE IF NOT EXISTS Users (user_id INTEGER PRIMARY KEY, user_name TEXT NOT NULL, phone TEXT)";
        std::string createBookAuthorsTableQuery = "CREATE TABLE IF NOT EXISTS BookAuthors (author_id INTEGER , book_id INTEGER , FOREIGN KEY (author_id) REFERENCES Authors(author_id), FOREIGN KEY (book_id) REFERENCES Books(book_id))";
        std::string createIssuedBooksTableQuery = "CREATE TABLE IF NOT EXISTS IssuedBooks (borrow_id INTEGER PRIMARY KEY, book_id INTEGER, user_id INTEGER, issueDate DATE NOT NULL, dueReturnDate DATE NOT NULL, FOREIGN KEY (book_id) REFERENCES Books(book_id), FOREIGN KEY (user_id) REFERENCES Users(user_id))";

        executeQuery(createAuthorsTableQuery);
        executeQuery(createBooksTableQuery);
        executeQuery(createUsersTableQuery);
        executeQuery(createBookAuthorsTableQuery);
        executeQuery(createIssuedBooksTableQuery);

    }

    
    void Add() {
        system("clear");
        int addChoice;
        do {
            std::cout << "\n***** Add Menu *****\n";
            std::cout << "1. Add Book\n";
            std::cout << "2. Add User\n";
            std::cout << "3. Issue Book\n";
            std::cout << "4. Back to Main Menu\n";
            std::cout << "\nEnter your choice: ";

            std::cin >> addChoice;
            std::cout << std::endl << "You entered " << addChoice << std::flush << std::endl;
            system("clear");
            switch (addChoice) {
                case 1:
                    system("clear");
                    addBook();
                    break;
                case 2:
                    system("clear");
                    addUser();
                    break;
                /*case 3:
                    system("clear");
                    IssueBook();
                    break;*/
                    
                case 3:
                    std::cout << "Returning to Main Menu... ";
                    break;
                default:
                    std::cout << "Invalid choice. Please enter a valid option.\n";
            }
        } while (addChoice != 3);
    }

   
    void Remove() {
        system("clear");
        int addChoice;
        do {
            std::cout << "\n***** Remove Menu *****\n";
            std::cout << "1. Remove Book\n";
            std::cout << "2. Remove User\n";
            std::cout << "3. Back to Main Menu\n";
            std::cout << "\nEnter your choice: ";

            std::cin >> addChoice;
            std::cout << std::endl << "You entered " << addChoice << std::flush << std::endl;

            switch (addChoice) {
                case 1:
                    system("clear");
                    removeBook();
                    break;
                case 2:
                    system("clear");
                    removeUser();                                                                                
                    break;
                case 3:
                    system("clear");
                    std::cout << "Returning to Main Menu... ";
                    break;
                default:
                    std::cout << "Invalid choice. Please enter a valid option.\n";
            }
        } while (addChoice != 3);
    }

    void Display() {
        system("clear");
        int addChoice;
        do {
            std::cout << "\n***** Display Menu *****\n";
            std::cout << "1. Display Books\n";
            std::cout << "2. Display Authors\n";
            std::cout << "3. Display Users\n";
            std::cout << "4. Display Issued Books\n";
            std::cout << "5. Back to Main Menu\n";
            std::cout << "\nEnter your choice: ";

            std::cin >> addChoice;
            std::cout << std::endl << "You entered " << addChoice << std::flush << std::endl;

            switch (addChoice) {
                case 1:
                    system("clear");
                    displayBooks();
                    break;
                case 2:
                    system("clear");
                    displayAuthors();
                    break;
                        
                case 3:
                    system("clear");
                    displayUsers();
                    break;
                case 4:
                    system("clear");
                    displayIssuedBooks();
                    break;
                case 5:
                    system("clear");
                    std::cout << "Returning to Main Menu... ";
                    break;
                default:
                    std::cout << "Invalid choice. Please enter a valid option.\n";
            }
        } while (addChoice != 5);
    }



    void addBook() {

        std::string title, author, section, isbn;
        int edition, floor;
        std::cin.ignore();

        std::cout << "Enter Book Title: ";
        std::getline(std::cin, title);

        std::cout << "Enter Author's Name: ";
        std::getline(std::cin, author);

        std::cout << "Enter Section: ";
        std::getline(std::cin, section);

        std::cout << "Enter Isbn No.: ";
        std::getline(std::cin, isbn);

        std::cout << "Enter Book Edition: ";
        std::cin >> edition;
        while (std::cin.fail()) {
            std::cerr << "Invalid input. Please enter an integer." << std::endl;
            std::cin.clear();  // Clear the fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
            std::cout << "Enter Book Edition: ";
            std::cin >> edition;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

     
        std::cout << "Enter Floor: ";
        std::cin >> floor;
        while (std::cin.fail()) {
            std::cerr << "Invalid input. Please enter an integer." << std::endl;
            std::cout << "Enter Floor: ";
            std::cin >> floor;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 


        std::string insertAuthorsDataQuery = "INSERT INTO Authors (name) VALUES ('" + author + "')";
        executeQuery(insertAuthorsDataQuery);
        sqlite3_int64 authorId = sqlite3_last_insert_rowid(db);

        std::string insertBooksDataQuery = "INSERT INTO Books (title, author_id, edition, floor, section, isbn) VALUES ('" +
                                        title + "', " + std::to_string(authorId) + ", " + std::to_string(edition) + ", " +
                                        std::to_string(floor) + ", '" + section + "', '" + isbn + "')";
        executeQuery(insertBooksDataQuery);
        sqlite3_int64 bookId = sqlite3_last_insert_rowid(db);

        std::string insertBookAuthorsDataQuery = "INSERT INTO BookAuthors (author_id, book_id) VALUES (" + std::to_string(authorId) + " ," + std::to_string(bookId) + ")";
        executeQuery(insertBookAuthorsDataQuery);
       
        std::cout << "Book added successfully." << std::endl;

        return;
    }

    void addUser() {
        std::string name, phone;

        std::cin.ignore();

        std::cout << "Enter User's Name: ";
        std::getline(std::cin, name);

        std::cout << "Enter Phone Number: ";
        std::getline(std::cin, phone);

        std::string insertUsersDataQuery = "INSERT INTO Users (user_name, phone) VALUES ('" +
                                           name + "', '" + phone + "')";

       executeQuery(insertUsersDataQuery);

        std::cout << "Data inserted into Users table successfully." << std::endl;

        return;
    }


    void IssueBook(){

        int book_id , user_id;
        std::string issueDate, dueReturnDate;

        std::cout << "Enter Book ID: ";
        std::cin >> book_id;
        while (std::cin.fail()) {
            std::cerr << "Invalid input. Please enter an integer." << std::endl;
            std::cout << "Enter Book id: ";
            std::cin >> book_id;
        }

        std::cout << "Enter User ID: ";
        std::cin >> user_id;
        while (std::cin.fail()) {
            std::cerr << "Invalid input. Please enter an integer." << std::endl;
            std::cout << "Enter User id: ";
            std::cin >> user_id;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter Borrow Date (YYYY-MM-DD): ";
        std::getline(std::cin, issueDate);

        std::cout << "Enter Due Return Date (YYYY-MM-DD): ";
        std::getline(std::cin, dueReturnDate);

        std::string insertIssueBooksDataQuery = "INSERT INTO IssuedBooks (book_id, user_id, issueDate, dueReturnDate) VALUES ('" +
        std::to_string(book_id) + "', '" + std::to_string(user_id) + "', '" + issueDate + "', '" + dueReturnDate + "')";

        executeQuery(insertIssueBooksDataQuery);

        std::cout<<"Issued Successfully...!\n";

        
    }


    void displayAuthors() {
        std::string query = "SELECT * FROM Authors";
        displayTable(query, "Authors");
    }

    void displayBooks() {
        std::string query = "SELECT * FROM Books";
        displayTable(query, "Books");
    }

    void displayUsers() {
        std::string query = "SELECT * FROM Users";
        displayTable(query, "Users");
    }

    void displayIssuedBooks() {
        std::string query = "SELECT * FROM IssuedBooks";
        displayTable(query, "IssuedBooks");
    }


    void displayTable(const std::string& query, const std::string& tableName) {
        std::cout << "Displaying data from table: " << tableName << std::endl;

        int result = sqlite3_exec(db, query.c_str(), displayCallback, 0, 0);

        if (result != SQLITE_OK) {
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            exit(1);
        }
    }

    
    static int displayCallback(void* data, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; ++i) {
            std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\t";
        }
        std::cout << std::endl;
        return 0;
    }

    void removeIssuedBook() {
    int borrow_id;
    std::cout << "Enter Borrow ID to remove: ";
    std::cin >> borrow_id;

    std::string removeIssuedBookQuery = "DELETE FROM IssuedBooks WHERE borrow_id = " + std::to_string(borrow_id);
    executeQuery(removeIssuedBookQuery);

    std::cout << "Issued book removed successfully." << std::endl;
    }

    void removeUser() {
    int user_id;
    std::cout << "Enter User ID to remove: ";
    std::cin >> user_id;

    std::string removeUserQuery = "DELETE FROM Users WHERE user_id = " + std::to_string(user_id);
    executeQuery(removeUserQuery);

    std::cout << "User removed successfully." << std::endl;
    }

    void removeBook() {
    int book_id;
    std::cout << "Enter Book ID to remove: ";
    std::cin >> book_id;

    std::string removeBookAuthorsQuery = "DELETE FROM BookAuthors WHERE book_id = " + std::to_string(book_id);
    executeQuery(removeBookAuthorsQuery);

    std::string removeBookQuery = "DELETE FROM Books WHERE book_id = " + std::to_string(book_id);
    executeQuery(removeBookQuery);

    std::cout << "Book removed successfully." << std::endl;
    }


    
};

int main() {
    LibraryManager library;

    std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[1m";
    std::cout << "Welcome To The Library" << std::endl << std::endl << std::endl << std::endl;

    std::cout << "\033[3m";
    std::cout << "Press Enter to continue..." << std::endl;

    std::cin.get();
    system("clear");

    std::cout << "\033[2J\033[1;1H";
    std::cout << "***** Library Management System  *****" << std::endl << std::endl;


    int choice;
    system("clear");
    do {
        std::cout << "\n***** Library Management System *****\n";
        std::cout << "1. Add\n";
        std::cout << "2. Remove\n";
        std::cout << "3. Display\n";
        std::cout << "4. Issue Book\n";
        std::cout << "5. Return Book\n";
        std::cout << "6. Exit\n";
        std::cout << "\nEnter your choice: ";

        std::cin >> choice;
        std::cout << std::endl << "You entered " << choice << std::flush << std::endl;

        switch (choice) {
            case 1:
                library.Add();
                break;
            case 2:
                library.Remove();
                break;
            case 3:
                library.Display();
                break;
            case 4:
                library.IssueBook();
                break;
            case 5:
                library.removeIssuedBook();
                break;
            case 6:
                std::cout << "Exiting... ";
                break;
            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
        }

    } while (choice != 6);



    std::cout << "Database closed." << std::endl;

    return 0;
}

 