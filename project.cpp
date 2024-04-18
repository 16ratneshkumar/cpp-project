#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <openssl/sha.h>

using namespace std;

int choice,total=0,aap=0,bjp=0,con=0;
int viewresult();
int main();
class Block {
private:
    int index;
    std::string previous_hash;
    std::string timestamp;
    std::string data;
    std::string hash;

    std::string calculate_hash() const {
        std::stringstream ss;
        ss << index << previous_hash << timestamp << data;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, ss.str().c_str(), ss.str().length());
        SHA256_Final(hash, &sha256);
        
        std::stringstream hash_ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            hash_ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return hash_ss.str();
    }

public:
    Block(int index, const std::string& previous_hash, const std::string& data)
        : index(index), previous_hash(previous_hash), data(data) {
        std::time_t now = std::time(nullptr);
        timestamp = std::ctime(&now);
        hash = calculate_hash();
    }

    std::string get_hash() const {
        return hash;
    }

    int get_index() const {
        return index;
    }
    
    std::string get_data() const {
    return data;
}

std::string get_timestamp() const {
    return timestamp;
}

};

class Blockchain {
private:
    std::vector<Block> chain;

public:
    Blockchain() {
        chain.emplace_back(0, "0", "default Block");
    }

    void add_block(const std::string& data) {
        int index = chain.size();
        const Block& previous_block = chain.back();
        chain.emplace_back(index, previous_block.get_hash(), data);
    }

    void print_chain() const {
        for (const Block& block : chain) {
            if (block.get_index()!=0){
            std::cout << "Voter #" << total<< std::endl;
            std::cout << "Hash: " << block.get_hash() << std::endl;
            std::cout << "Timestamp: " << block.get_timestamp() << std::endl;
            std::cout << std::endl;
            }
        }
    }
};

bool checkvoter(const std::string& searchvoter) {
    std::ifstream file("voter.txt");
    if (!file.is_open()) {
        std::cerr << "Unable to open file: "<<std::endl;
        return false;
    }

    std::string voterid;
    while (std::getline(file, voterid)) {
        if (voterid.find(searchvoter) != std::string::npos) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

int vote(){
    std::cout<<"1.Aap\n2.bjp\n3.congress "<<endl;
    std::cin>>choice;
    switch (choice){
        case 1:
            aap+=1;
            total+=1;
            break;
        case 2:
            bjp+=1;
            total+=1;
            break;
        case 3:
            con+=1;
            total+=1;
            break;
        default:
            std::cout<<"invaild input vote again!"<<endl;
            vote();
    }
    Blockchain blockchain;
    blockchain.add_block("Voter Data");
    blockchain.print_chain();
}

int viewresult(){
    std::cout<<"Final Result of election"<<endl;
    std::cout<<"Total vote:: "<<total<<endl;
    std::cout<<"Aap vote:: "<<aap<<endl;
    std::cout<<"bjp vote:: "<<bjp<<endl;
    std::cout<<"congress vote:: "<<con<<endl;return 0;;
}

int addvoter() {
    std::ofstream outputFile("voter.txt"); 
    if (outputFile.is_open()) {
        std::string voterid ;
        std::cout<<"enter voter id";
        std::cin>>voterid;
        outputFile << voterid; 
        outputFile.close(); 
        std::cout << "Thank you.You are register successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
    return 0;
}
    

int main() {
    std::string voterid;
    int num;
    std::cout<<"1.You are an admin\n2.You are a voter"<<std::endl;
    std::cin>>num;
    switch (num){
        case 1:
            std::cout<<"1.add voter\n2.view result"<<std::endl;
            std::cin>>num;
            switch (num){
                case 1:
                    addvoter();
                    break;
                 case 2:
                     viewresult();
                     main();
                     break;
                 default :
                     std::cout<<"invaild input"<<std::endl;
            }
        case 2:
            std::cout<<"Enter your voter id:: ";
            std::cin >>voterid;
            if (checkvoter( voterid)) {
                vote();
            } else {
                std::cout << "You are not register voter." << std::endl;
            }
            main();
    return 0;
    }
}
