#include "cryptocertificate.hpp"
uint64_t account_to_uint64(account_name user){
	auto namecheck = eosio::name{user};
	uint64_t Id = eosio::string_to_name(namecheck.to_string().c_str());
	return Id;
}
void cryptocertificate::initwl(){
	auto itr = whitelist.begin();
	while (itr != whitelist.end()){
		eosio::print(itr->id," ");
		itr = whitelist.erase(itr);
	}
	whitelist.emplace(_self,[&](auto &o){
		o.id = N(shzshzshzshz);
	});
}

int cryptocertificate::qwhitelist(account_name user){
	uint64_t id = account_to_uint64(user);
	auto itr = whitelist.find(id);
	if (itr == whitelist.end()) return 0;
	else return 1;
}

void cryptocertificate::adduser(account_name user,uint32_t role){
	uint64_t Id = account_to_uint64(user);
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
	require_auth(user);
	uint64_t id =  account_to_uint64(user);
	auto lookup = data.find(id);
	data.erase(lookup);
	eosio::print(eosio::name{user}," has deleted !");
}

void cryptocertificate::crtcert(
	account_name user,
	std::string crtname,
	std::string from,
	std::string to,
	std::string content
	){
	require_auth(user);
}
