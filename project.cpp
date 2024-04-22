// Include necessary header files
#include <iostream> // For input and output
#include <ctime> // For getting the current time
#include <vector> // For using the vector container
#include <string> // For using the string class
#include <sstream> // For string stream operations
#include <fstream> // For file stream operations
#include <openssl/sha.h> // For SHA256 hashing
#include <iomanip> // For formatting output

// Use the standard namespace
using namespace std;

// Global variables to store the vote counts and total votes
int choice, total = 0, aap = 0, bjp = 0, con = 0;

// Function prototypes
int viewresult();
int main();

// Class to represent a block in the blockchain
class Block {
private:
    int index; // Index of the block in the chain
    string previous_hash; // Hash of the previous block
    string timestamp; // Timestamp for when the block is created
    string data; // Data contained in the block
    string hash; // Hash of the current block

    // Function to calculate the hash of the block
    string calculate_hash() const {
        stringstream ss; // String stream to concatenate block information
        ss << index << previous_hash << timestamp << data; // Add block information to the stream
        unsigned char hash[SHA256_DIGEST_LENGTH]; // Array to store the hash result
        SHA256_CTX sha256; // SHA256 context
        SHA256_Init(&sha256); // Initialize the SHA256 context
        SHA256_Update(&sha256, ss.str().c_str(), ss.str().length()); // Update the context with the block information
        SHA256_Final(hash, &sha256); // Finalize the hash
        
        stringstream hash_ss; // String stream to format the hash
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            hash_ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]); // Format each byte of the hash
        }
        return hash_ss.str(); // Return the formatted hash as a string
    }

public:
    // Constructor for the Block class
    Block(int idx, const string& prev_hash, const string& data)
        : index(idx), previous_hash(prev_hash), data(data) {
        time_t now = time(nullptr); // Get the current time
        timestamp = ctime(&now); // Convert the time to a string
        hash = calculate_hash(); // Calculate the hash of the block
    }

    // Getter functions to access private attributes
    string get_hash() const { return hash; }
    int get_index() const { return index; }
    string get_data() const { return data; }
    string get_timestamp() const { return timestamp; }
};

// Class to represent the blockchain
class Blockchain {
private:
    vector<Block> chain; // Vector to store the chain of blocks

public:
    // Constructor for the Blockchain class
    Blockchain() {
        // Create the genesis block and add it to the chain
        chain.emplace_back(0, "0", "Genesis Block");
    }

    // Function to add a new block to the blockchain
    void add_block(const string& data) {
        int index = chain.size(); // Get the next index for the new block
        const Block& previous_block = chain.back(); // Get the last block in the chain
        chain.emplace_back(index, previous_block.get_hash(), data); // Create and add the new block
    }

    // Function to print the entire blockchain
    void print_chain() const {
        for (const Block& block : chain) {
            if (block.get_index() != 0) { // Skip the genesis block
                cout << "Voter #" << total << endl; // Print the voter number
                cout << "Hash: " << block.get_hash() << endl; // Print the hash of the block
                cout << "Timestamp: " << block.get_timestamp() << endl; // Print the timestamp
                cout << endl;
            }
        }
    }
};

// Function to check if a voter is registered
bool checkvoter(const string& searchvoter) {
    ifstream file("voter.txt"); // Open the voter file
    if (!file.is_open()) {
        cerr << "Unable to open file: " << endl; // Print an error if the file cannot be opened
        return false;
    }

    string voterid; // Variable to store voter IDs from the file
    while (getline(file, voterid)) { // Read voter IDs from the file
        if (voterid.find(searchvoter) != string::npos) { // Check if the voter ID is in the file
            file.close(); // Close the file
            return true; // Return true if the voter is registered
        }
    }

    file.close(); // Close the file
    return false; // Return false if the voter is not registered
}

// Function to handle the voting process
int vote() {
    cout << "1.Aap\n2.bjp\n3.congress " << endl; // Print the voting options
    cin >> choice; // Get the user's vote
    switch (choice) { // Handle the vote based on the user's choice
        case 1: // If the user votes for Aap
            aap += 1; // Increment the Aap vote count
            total += 1; // Increment the total vote count
            break;
        case 2: // If the user votes for BJP
            bjp += 1; // Increment the BJP vote count
            total += 1; // Increment the total vote count
            break;
        case 3: // If the user votes for Congress
            con += 1; // Increment the Congress vote count
            total += 1; // Increment the total vote count
            break;
        default: // If the user enters an invalid option
            cout << "Invalid input, vote again!" << endl; // Prompt the user to vote again
            vote(); // Recursive call to the vote function
    }
    Blockchain blockchain; // Create a new blockchain instance
    blockchain.add_block("Voter Data"); // Add a new block with the vote data
    blockchain.print_chain(); // Print the updated blockchain
    return 0; // Return 0 to indicate success
}

// Function to view the election results
int viewresult() {
    cout << "Final Result of election" << endl; // Print the header for the results
    cout << "Total votes: " << total << endl; // Print the total number of votes
    cout << "Aap votes: " << aap << endl; // Print the number of votes for Aap
    cout << "BJP votes: " << bjp << endl; // Print the number of votes for BJP
    cout << "Congress votes: " << con << endl; // Print the number of votes for Congress
    return 0; // Return 0 to indicate success
}

// Function to add a new voter to the registry
int addvoter() {
    ofstream outputFile("voter.txt", ios::app); // Open the voter file in append mode
    if (outputFile.is_open()) { // Check if the file is open
        string voterid; // Variable to store the new voter ID
        cout << "Enter voter ID: "; // Prompt the user to enter a voter ID
        cin >> voterid; // Get the voter ID from the user
        outputFile << voterid << endl; // Write the voter ID to the file
        outputFile.close(); // Close the file
        cout << "Thank you. You are registered successfully." << endl; // Print a success message
    } else {
        cerr << "Unable to open file for writing." << endl; // Print an error if the file cannot be opened
    }
    return 0; // Return 0 to indicate success
}

// Main function to run the voting system
int main() {
    string voterid; // Variable to store the voter ID
    int num; // Variable to store the user's menu choice
    cout << "1.You are an admin\n2.You are a voter" << endl; // Print the main menu options
    cin >> num; // Get the user's menu choice
    switch (num) { // Handle the user's menu choice
        case 1: // If the user is an admin
            cout << "1.Add voter\n2.View result" << endl; // Print the admin menu options
            cin >> num; // Get the admin's choice
            switch (num) { // Handle the admin's choice
                case 1: // If the admin chooses to add a voter
                    addvoter(); // Call the function to add a voter
                    break;
                case 2: // If the admin chooses to view the results
                    viewresult(); // Call the function to view the results
                    break;
                default: // If the admin enters an invalid option
                    cout << "Invalid input" << endl; // Print an error message
            }
            break;
        case 2: // If the user is a voter
            cout << "Enter your voter ID: "; // Prompt the user to enter their voter ID
            cin >> voterid; // Get the voter ID from the user
            if (checkvoter(voterid)) { // Check if the voter is registered
                vote(); // Call the function to handle the voting process
            } else {
                cout << "You are not a registered voter." << endl; // Inform the user they are not registered
            }
            break;
        default: // If the user enters an invalid option at the main menu
            cout << "Invalid input. Please enter 1 for admin or 2 for voter." << endl; // Print an error message
    }
    return 0; // Return 0 to indicate success
}