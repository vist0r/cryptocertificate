#include "cryptocertificate.hpp"
int cryptocertificate::qwhitelist(account_name user){
	return 0;
}

void cryptocertificate::adduser(account_name user,uint32_t role){
	auto namecheck = eosio::name{user};
	uint64_t Id = eosio::string_to_name(namecheck.to_string().c_str());
	auto itr = data.find(Id);
	eosio_assert(itr == data.end(),"Account already exists!");
	int hasPermission = !(!(cryptocertificate::qwhitelist(user)) && (role > 1));
	eosio_assert(hasPermission ,"Permission denied!!" );
	data.emplace(user,[&](auto &o){
		std::vector<std::string> v;
		v.clear();
		o.role = role;
		auto uname = eosio::name{user};
		o.uname = uname.to_string();
		o.id = eosio::string_to_name(o.uname.c_str());
		o.cname = v;
	});
	eosio::print(user," is created! ") ;
}

void cryptocertificate::udestroy(account_name user){
	auto uname = eosio::name{user};
	std::string name = uname.to_string();
	uint64_t id = eosio::string_to_name(name.c_str());
	auto lookup = data.find(id);
	data.erase(lookup);
	eosio::print(name," has deleted !");
}
