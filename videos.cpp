#include <iostream> // for file and stdin and stdout I/O
#include <fstream> // for file I/O
//for system() 
#include <stdlib.h>
// for strings and vectors
#include <string.h>
#include <vector>
// for curl 
#include <curl/curl.h>
#include <curl/easy.h>
// for stat 
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

// for libcurl 
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int main(int argc, char* argv[])
{
	struct stat sb;
	FILE* first;
    first = fopen("html.txt","r");
    fseek(first, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(first);
    if (len == 0)   //check if the file is empty or not. if html.txt is empty, it is the first run. So include code to set downloads directory here.  
    {
    	std::ofstream path;
    	path.open("download.txt");
    	std::cout<<"Please specify the entire path of the downloads folder\n";
    	std::string location;
    	std::cin>>location;
    	const char* clocation = location.c_str();
    	if (stat(clocation, &sb) == -1) 
    	{
    		cout<<"This is an invalid directory\nExiting program\n";
    		return 1;
    	} 
    	location.append("/%(title)s-%(id)s.%(ext)s");
    	path<<location;
    	path.close();
    }
    fclose(first);
	
	// if user enters a path at any time, make that the downloads folder
	
	if(argc == 3)
    {
    	// open downloads.txt and update 
    	if (stat(argv[2], &sb) == -1) 
    	{
    		cout<<"This is an invalid directory\nExiting program\n";
    		return 1;
    	}
        std::string S(argv[2]);
    	S.append("/%(title)s-%(id)s.%(ext)s");
    	std::ofstream update;
    	update.open("download.txt",std::ios::out);
    	update<<S;
    	update.close();
    }
    if(argc != 2 && argc !=3)
	{
		std::cout<<"Usage: ./a.out <url of youtube playlist> OR ./a.out <url of youtube playlist> <path of downloads folder> \n";
		return 1;
	}

	if(stat("/usr/share/man/man1/youtube-dl.1.gz", &sb) == -1)
	{
		std::cout<<"The required tool youtube-dl is not installed!\nPlease execute the following commands to install\n";
		std::cout<<">>sudo apt-get update\n>>sudo apt-get install youtube-dl\n";
		return 1;
	}

	if(stat("/usr/share/man/man1/curl.1.gz", &sb) == -1)
	{
		std::cout<<"The required tool curl is not installed!\nPlease execute the following commands to install\n";
		std::cout<<">>sudo apt-get update\n>>sudo apt-get install libcurl4-gnutls-dev \n";
		return 1;
	}

	// get the html of a youtube playlist page and save in text file 
	CURL *curl_handle;
    static const char *headerfilename = "html.txt";
    FILE *headerfile;
    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* set URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]);

    /* no progress meter please */
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    /* open the files */
    headerfile = fopen(headerfilename,"w");
    if (headerfile == NULL) 
    {
      curl_easy_cleanup(curl_handle);
      return -1;
    }

    /* we want the headers to this file handle */
    curl_easy_setopt(curl_handle,   CURLOPT_WRITEDATA, headerfile);

    /*
     * Notice here that if you want the actual data sent anywhere else but
     * stdout, you should consider using the CURLOPT_WRITEDATA option.  */

    /* get it! */
    curl_easy_perform(curl_handle);

    /* close the header file */
    fclose(headerfile);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);


    // parse through the html file and extract video links, save in a vector 
	std::ifstream input;
	
	input.open("html.txt");
	if(!input)
	{
		std::cout<<"Could not open file\n";
		return 1;
	}
	std::string str;
	int i=0;
	vector <string> address;
	while(!input.eof())
	{
		getline(input,str);

		std::size_t pos = str.find("<a href=\"/watch");
		if(pos != std::string::npos)
		{
		    std::string link;
		    pos+=9;
		    while(str[pos] != '"')
		    {
			  link.push_back(str[pos]);
			  ++pos;
		    }
		    link.push_back('\0');
		    
		    std::string url = "http://www.youtube.com";
		    url.append(link);
		    address.push_back(url);
		}
	}
	input.close();

	// writing link addresses to a file, downloading using youtube-dl 
	std::ofstream output;
	output.open("address.txt");
	for(i=1; i<address.size(); ++i) // to skip the url for playing all videos, i.e, the PLAY ALL button
	{
		output << address[i] <<"\n";
		std::string command = "youtube-dl -i -c --output '"; 
		std::string folder;
		std::ifstream loc;
		loc.open("download.txt");
		loc >> folder;
		loc.close();
		command.append(folder);
		command.append("' ");
		command.append(address[i]);
		const char* cstr = command.c_str();
		//std::cout<<cstr<<"\n";
		system(cstr);
	}
	output.close();

	// opening and reading file contents 
	std::ifstream reader;
	reader.open("address.txt");
	std::cout<<"\nFile contents\n";
	while(!reader.eof())
	{
		getline(reader, str);
		std::cout<<str<<"\n";
	}
	reader.close();
	return 0;
}
