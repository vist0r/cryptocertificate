#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include "cryptocertificate.hpp"
bool cryptocertificate::qwhitelist(account_name user){
	return true;
}
void cryptocertificate::adduser(uint64_t id,account_name user,uint32_t role){
	auto namecheck = eosio::name{user};
	uint64_t Id = eosio::string_to_name(namecheck.to_string().c_str());
	auto itr = data.find(Id);
	eosio_assert(itr == data.end(),"Account already exists!");
	eosio_assert( (!qwhitelist(user)) && (role > 1) ,"Permission denied!" );
	data.emplace(user,[&](auto &o){
		std::vector<std::string> v;
		v.clear();
		o.role = role;
		auto uname = eosio::name{user};
		o.uname = uname.to_string();
		o.id = eosio::string_to_name(o.uname.c_str());
		o.cname = v;
	});
	eosio::print(user," is created! ");
}