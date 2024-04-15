//
// Created by Hassan on 31/03/2024.
//
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

using namespace std;

using namespace std;
using json = nlohmann::json;

string apiKey = "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";

// Function prototype for writing callback
size_t WriteCallback(void*, size_t, size_t, string*);

// Function to send a request to GPT API
string getGPTResponse(const string& prompt, const string& apiKey) {
    string baseUrl = "https://api.openai.com/v1/chat/completions";
    string response;
    CURL* curl = curl_easy_init();

    if (curl) {
        json requestData;
        requestData["model"] = "gpt-3.5-turbo-1106";
        requestData["messages"][0]["role"] = "user";
        requestData["messages"][0]["content"] = prompt;
        requestData["temperature"] = 0;

        string requestDataStr = requestData.dump();

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestDataStr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestDataStr.length());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);

        json jresponse = json::parse(response);
        // If have  to trouble shoot the JSON request:
//        std::cout << "Raw JSON Response: " << jresponse.dump() << std::endl;
        // Ensure all fields exist:
        if (jresponse.contains("choices") &&
            jresponse["choices"].is_array() &&
            !jresponse["choices"].empty() &&
            jresponse["choices"][0].contains("message") &&
            jresponse["choices"][0]["message"].contains("content")) {

            return jresponse["choices"][0]["message"]["content"].get<string>();
        } else {
            return "Error: Unexpected response structure from OpenAI API.";
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    json jresponse = json::parse(response);
    return jresponse["choices"][0]["message"]["content"].get<string>();
}

// Callback function for writing data received from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

//int main() {
//    // Prompt user for API key
//
//
//    // Prompt user for model (optional)
////    cout << "Enter model (default: gpt-3.5-turbo): ";
////    string model = "gpt-3.5-turbo-instruct";
////    getline(cin, model);
//
//    // Main loop to interact with the chatbot
//    while (true) {
//        cout << "You: ";
//        string prompt;
//        getline(cin, prompt);
//
//        // Break the loop if user enters "exit"
//        if (prompt == "exit")
//            break;
//
//        // Get GPT response
//        string response = getGPTResponse(prompt, apiKey);
//        cout << "Bot: " << response << endl;
//    }
//
//    return 0;
//}


int main() {

    ifstream promptFile("/Users/hassan/CLionProjects/OOPSPROJ/IBAText.txt");

    if (!promptFile.is_open()) {
        cerr << "Error: Unable to open file \n";
        return 1;
    }


    // Main loop to interact with the chatbot
    string promptEnhancer;
    string prompt;
    string modifiedPrompt;
    string line;

    while (getline(promptFile, line)) {
        getline(promptFile, line);
        promptEnhancer += line; // Append each line to the prompt string

    }
    while (true) {
        cout << "You: ";
        getline(cin, prompt);

        if (prompt == "") {
            cerr << "It seems like your message is incomplete. Could you please provide more details or let me know if there's anything you need assistance with?";
        }

        if (prompt == "exit")
            break;

        modifiedPrompt = prompt + promptEnhancer;

        string response = getGPTResponse(modifiedPrompt, apiKey);
        cout << "Bot: " << response << endl;
        // Break the loop if user enters "exit"
    }

    promptFile.close();

    return 0;

}

//Answer to anything related to IBA should be answered from the text below:
//Pretend to be fine tuned on this text, Therefore behave as if you are fine tuned on this content therefore answer any question which may be related to this text. Also please do not reply to this by saying that you are thankful to be provided with this information, instead jump right into action.
//Also only answer questions about IBA from the provided jsonl: