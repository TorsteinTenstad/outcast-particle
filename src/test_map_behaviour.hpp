

class Container
{
public:
	std::string s = "default string";

	Container()
	{
		s = "set by default constructor";
	}

	Container(std::string ss)
	{
		s = ss;
	}
};

int main()
{
	std::map<int, Container> containers;
	containers[0] = {};
	containers[1] = { "Hei" };
	containers[2].s = "Wow";
	containers[7];

	for (auto x : containers)
	{
		std::cout << x.first << ", ";
	}
	std::cout << std::endl;

	std::cout << containers[0].s << std::endl;
	std::cout << containers[1].s << std::endl;
	std::cout << containers[2].s << std::endl;
	std::cout << containers[3].s << std::endl;

	std::map<int, int> ints;
	std::cout << ints[0] << std::endl;

	return 0;
}