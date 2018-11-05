#include "network.h"
#include "random.h"
#include <algorithm>

void Network::resize(const size_t& t){
	
	values.resize(t);
	
	for (unsigned int i(0); i<values.size(); ++i){
		values[i]=RNG.normal(0,1);
	}
}

bool Network::add_link(const size_t& a, const size_t& b){
	
	if(a>=values.size() or b>=values.size() or a==b){
		return false;
	}
	
	for (std::multimap<size_t,size_t>::const_iterator i=links.begin(); i!=links.end(); ++i){
		if ((i->first==a and i->second==b) or (i->first==b and i->second==a)){
			return false;
		}
	}

	links.insert(std::pair<size_t,size_t> (a,b));
	links.insert(std::pair<size_t,size_t> (b,a));
	
	return true;
}
 
size_t Network::random_connect(const double& mean_deg){
	
	links.erase(links.begin(), links.end());
	int count(0);
	
	for (size_t n=0; n<values.size(); ++n){
		
		//number of links the node n will create
		int degree = RNG.poisson(mean_deg);
		std::vector<int> possiblenodes(degree);
		
		//name of the nodes connected to node n
		RNG.uniform_int(possiblenodes, 0, values.size());
		
		for (size_t i(0); i<possiblenodes.size(); ++i){
			if (add_link(n,possiblenodes[i])) { ++count; };
			
		}
	}
	return count;
}
 
size_t Network::set_values(const std::vector<double>& newvalues){
	
	int count(0);
	
	if (newvalues.size() >values.size()){
		for (unsigned int i(0); i<values.size(); ++i){
			values[i]= newvalues[i];
			++count;
		}
	}
	
	if (newvalues.size()<=values.size()){
		for (unsigned int i(0); i<newvalues.size(); ++i){
			values[i]= newvalues[i];
			++count;
		}
	}
	return count;
}
 
size_t Network::size() const{
	return values.size();
}

size_t Network::degree(const size_t &n) const{
	size_t degree = links.count(n);
	return degree;
}

double Network::value(const size_t &n) const{
	if (n<values.size()){
		return values[n];
	}
	else {return 0;}
}

std::vector<double> Network::sorted_values() const{
	
	std::vector<double>copy;
	
	for (size_t i(0); i<values.size(); ++i){
		copy.push_back(values[i]);
	}
	
	std::sort(copy.begin(), copy.end());
	std::reverse(copy.begin(), copy.end());
	
	return copy;
}

std::vector<size_t> Network::neighbors(const size_t& n) const{
	
	std::vector<size_t> linkstonode;
	size_t a(0);
	
	for (std::multimap<size_t,size_t>::const_iterator i=links.begin(); i!=links.end(); ++i){
		if (i->first==n){
			a=i->second;
			linkstonode.push_back(a);
		}
	}
	return linkstonode;
}
