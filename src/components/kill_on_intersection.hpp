#pragma once


enum KillOnIntersectionChannels
{
	KillPlayer,
	CollectCoin
};

class KillOnIntersection
{
	std::map<KillOnIntersectionChannels, bool> kills;
	std::map<KillOnIntersectionChannels, bool> dies;
};
