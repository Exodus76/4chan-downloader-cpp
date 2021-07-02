#include <string>
#include <iostream>
#include <regex>
#include <filesystem>

#include <cpr/cpr.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "No link passed";
        return 0;
    };

    std::string link = argv[1];

    std::regex cdn("boards.4chan.org|boards.4channel.org");
    std::string main_link;
    std::regex_replace(back_inserter(main_link), link.begin(), link.end(), cdn, "a.4cdn.org");
    std::regex board("a.4cdn.org/(.+?)/thread/([0-9]*)");
    std::smatch m;
    std::regex_search(main_link, m, board);
    std::string board_name = m[1];
    std::string thread_id = m[2];

    if (fs::exists(thread_id))
    {
        std::cout << "folder named " << thread_id << " already exists" << std::endl;
        return 0;
    }
    else fs::create_directory(thread_id);

    cpr::Response r = cpr::Get(cpr::Url{ main_link + ".json" });

    json json_content;
    json_content = json::parse(r.text);

    auto total_replies = stoi(json_content["posts"][0]["replies"].dump());

    auto session = cpr::Session();
    std::string fname, tim, ext, image_url = "https://i.4cdn.org/" + board_name;

    std::cout << "downloading " << total_replies << " files" << std::endl;
    for (int i = 1; i <= total_replies; ++i)
    {
        if (json_content["posts"][i].contains("tim"))
        {
            fname = json_content["posts"][i]["filename"];
            tim = json_content["posts"][i]["tim"].dump();
            ext = json_content["posts"][i]["ext"];

            auto ofstream = std::ofstream(thread_id + "\\" + fname + ext, std::ios::binary);
            session.SetUrl(cpr::Url{ image_url + "/" + tim + ext });
            auto response = session.Download(ofstream);
            ofstream.close();
        }
    }
    
    return EXIT_SUCCESS;

}

//Download function but in c
//i.4cdn.org/[board]/[4chan image ID].[file extension]

/*
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
CURL* curl;
FILE* fp;
CURLcode res;
std::string s = "https://i.4cdn.org/wg/1623682113463.png";
char* url = const_cast<char*>(s.c_str());
char outfilename[FILENAME_MAX] = "bbb.png";
curl = curl_easy_init();
if (curl) {
    fopen_s(&fp, outfilename, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    //always cleanup
    curl_easy_cleanup(curl);
    fclose(fp);
}
return 0;
*/
