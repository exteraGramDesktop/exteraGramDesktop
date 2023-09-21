
#include <mutex>
#include "rabbit/openai/curl/curl.h"
#include "rabbit/openai/nlohmann/json.hpp"
#include "rabbit/rabbit_settings.h"

using json = nlohmann::json;

std::string apiKey = RabbitSettings::JsonSettings::GetString("openai_key").toStdString(), inputText = "";

static int aiStyle = 0;

int maxTokens = 1024, n = 1, bestOf = 1;
float temperature = 0.7f, presencePenalty = 0.0f;
bool echo = false, waiting = false;

const char* modelOptions[] = { "text-davinci-003", "text-curie-001", "text-babbage-001", "text-ada-001", "code-davinci-002", "code-cushman-001" }; // code-davinci-002 & code-cushman-001 models seem to have issues
int modelIndex = 0;

const std::array<std::string, 11> promptStrings = {
    "", // Default
    "You are going to write a professional and smart email: ",
    "You need to explain a topic to a non-technical audience. Please write a concise explanation: ",
    "You are going to write a short story. Please let your imagination run wild and write a captivating story: ",
    "You are going to write a news article. Please write a compelling and informative news article: ",
    "You need to write an academic paper. Please provide a well-researched and insightful paper: ",
    "You need to write a marketing copy for a product or service. Please write a persuasive and engaging marketing copy: ",
    "You need to write a social media post. Please write a catchy and attention-grabbing post: ",
    "You need to write C++ Window Kernel Driver code, Do not forgot this is for windows kernel driver. Please write a clear and correct code: ",
    "You need to write a legal document. Please write a clear and concise legal document: ",
    "You need to write a professional resume for a job position. Please write a well-structured and impressive resume: "
};

std::mutex sendMutex;
CURL* httpClient;

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    size_t num_bytes = size * nmemb;
    std::string* response = static_cast<std::string*>(userdata);
    response->append(ptr, num_bytes);
    return num_bytes;
}

std::string SendRequest(std::string inputText)
{
    std::lock_guard<std::mutex> lock(sendMutex);
    std::string response;

    waiting = true;

    httpClient = curl_easy_init();
    if (httpClient == nullptr)
    {
        std::string errorCode = "[-] Failed to initialize curl";
        return "error in openai.h: " + errorCode;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::stringstream header;
    header << "Authorization: Bearer " << apiKey;
    headers = curl_slist_append(headers, header.str().c_str());
    curl_easy_setopt(httpClient, CURLOPT_HTTPHEADER, headers);

    // Set the URL to the OpenAI API endpoint
    std::string apiUrl = "https://api.openai.com/v1/engines/" + std::string(modelOptions[modelIndex]) + "/completions";
    curl_easy_setopt(httpClient, CURLOPT_URL, apiUrl.c_str());

    // Set the callback function for libcurl
    curl_easy_setopt(httpClient, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(httpClient, CURLOPT_WRITEFUNCTION, WriteCallback);

    // Set the prompt based on the selected AI style
    if (aiStyle >= 0 && aiStyle < promptStrings.size()) {
        inputText.insert(0, promptStrings[aiStyle]);
    }

    // Send a request
    json requestData =
    {
        { "prompt", inputText },
        { "max_tokens", maxTokens },
        { "temperature", temperature },
        { "n", n },
        { "presence_penalty", presencePenalty },
        { "best_of", bestOf },
        { "echo", echo }
    };
    std::string requestDataString = requestData.dump();
    curl_easy_setopt(httpClient, CURLOPT_POSTFIELDS, requestDataString.c_str());

    CURLcode res = curl_easy_perform(httpClient);
    if (res != CURLE_OK)
    {
        curl_slist_free_all(headers);
        std::string errorCode = "[-] Failed with error code: " + std::string(curl_easy_strerror(res)) + "\n";
        waiting = false;

        return "error in openai.h: " + errorCode;
    }

    long http_code = 0;
    res = curl_easy_getinfo(httpClient, CURLINFO_RESPONSE_CODE, &http_code);
    if (res != CURLE_OK)
    {
        curl_slist_free_all(headers);
        std::string errorCode = "[-] Failed to get HTTP response code:" + std::string(curl_easy_strerror(res)) + "\n";

        waiting = false;

        return "error in openai.h: " + errorCode;
    }

    if (http_code != 200)
    {
        curl_slist_free_all(headers);

        std::stringstream ss;
        ss << "[-] Failed with HTTP error code: " << http_code << "\n";
        std::string errorCode = ss.str();

        waiting = false;

        return "error in openai.h: " + errorCode;
    }

    // Trim whitespaces
    response.erase(response.find_last_not_of(" \n\r\t") + 1);
    response.erase(0, response.find_first_not_of(" \n\r\t"));

    // Parse the response as JSON
    json responseData = json::parse(response);

    //logEditor.InsertText(responseData.dump() + "\n");
     
    // Extract the "text" value from the response
    std::string completionText = responseData["choices"][0]["text"];

    std::string trimmedResponse = completionText;
    trimmedResponse.erase(0, trimmedResponse.find_first_not_of("\n\r\t "));
    trimmedResponse.erase(trimmedResponse.find_last_not_of("\n\r\t ") + 1);

    curl_slist_free_all(headers);

    inputText.erase();

    waiting = false;

    return trimmedResponse;
}