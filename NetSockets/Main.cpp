import NetSocket;
import <vector>;
import <string>;

using namespace std;

int main()
{
	// Example usage:
	string exampleUrl = "www.google.com";
	printf("Using example URL: %s\r\n", exampleUrl.c_str()); // NOTE: Not using std::format right now, VS compiler still having trouble with this as of now...

	NetSocket net_client(exampleUrl.c_str(), 80);
	if (net_client.bConnected)
		printf("NOTE: We are connected!\r\n");

	if (!net_client.Send("GET / HTTP/1.1\r\nHost: " + exampleUrl + "\r\nConnection: close\r\n\r\n"))
	{
		printf("Failure to send GET request to target %s. (No internet?)\r\n", exampleUrl.c_str());
		//cout << std::format("Failure to send GET request to target {}. (No internet?)\n", exampleUrl); // C++20-way
		return 1;
	}

	vector<uint8_t> received;
	net_client.Receive(received);
	
	printf("Received:\r\n%s\r\n", string(received.begin(), received.end()).c_str());
	return 0;
}