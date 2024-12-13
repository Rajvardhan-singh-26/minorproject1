#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

int main() {
    try {
        // Initialize the MongoDB driver
        mongocxx::instance instance{}; // This should be done only once in the lifetime of your app
        
        // Connect to MongoDB (use default localhost:27017)
        mongocxx::client conn{mongocxx::uri{}};
        
        // Access a database (creating it if it doesn't exist)
        mongocxx::database db = conn["test_database"];
        
        // Access a collection (creating it if it doesn't exist)
        mongocxx::collection coll = db["test_collection"];
        
        // Insert a document into the collection
        bsoncxx::builder::stream::document document{};
        document << "name" << "John Doe"
                 << "age" << 30
                 << "city" << "New York";
        
        coll.insert_one(document.view());

        std::cout << "Document inserted successfully!" << std::endl;

        // Query the collection
        auto cursor = coll.find({});
        for (auto&& doc : cursor) {
            std::cout << bsoncxx::to_json(doc) << std::endl; // Convert BSON document to JSON string
        }

    } catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Exception: " << e.what() << std::endl;
    }

    return 0;
}
