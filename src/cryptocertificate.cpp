#include "cryptocertificate.hpp"

#define EOS_SYMBOL S(4,EOS)

const time ONEDAY = 24 * 3600;
uint64_t account_to_uint64(account_name user){
	auto namecheck = eosio::name{user};
	uint64_t Id = eosio::string_to_name(namecheck.to_string().c_str());
	return Id;
}
void cryptocertificate::init(){
	require_auth(_self);
	auto itr = whitelist.begin();
	while (itr != whitelist.end()){
		eosio::print(itr->id," ");
		itr = whitelist.erase(itr);
	}
	whitelist.emplace(_self,[&](auto &o){
		o.id = N(shzshzshzshz);
	});
	auto itrc = certid.get_or_create(_self,certId{-1});
	eosio::print("time = ", now());
}

int cryptocertificate::qwhitelist(account_name user){
	uint64_t id = account_to_uint64(user);
	auto itr = whitelist.find(id);
	if (itr == whitelist.end()) return 0;
	else return 1;
}

void cryptocertificate::adduser(account_name user,uint32_t role){	
	userInfoIndex data(_self,user);
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
	eosio::print(user," is created! ");
}

void cryptocertificate::udestroy(account_name user){
	require_auth(user);
	userInfoIndex data(_self,user);
	uint64_t id =  account_to_uint64(user);
	auto lookup = data.find(id);
	data.erase(lookup);
	eosio::print(eosio::name{user}," has deleted !");
}
uint64_t cryptocertificate::get_unit_price(){
	return 1000;
}

void cryptocertificate::crtr(
	account_name user,
	account_name to,
	std::string crtname,
	std::string content,
	eosio::extended_asset amt
	){
	
//	eosio::print(amt.quantity.symbol ," " , EOS_SYMBOL);
	//eosio_assert(cryptocertificate::get_unit_price() * content.length() == amt.amount,"Invaild amount!");
	eosio_assert(amt.contract == N(eosio.token), "Only true EOS token is allowed!");
//	eosio_assert(amt.symbol.name() == EOS_SYMBOL,"Only true EOS token is allowed!");
	certIndex certu(_self,user);
	certIndex certt(_self,to);
	singleton_pertime perqtime(_self,user);
	if (!perqtime.exists()){
		auto itrper = perqtime.get_or_create(_self,perq{(uint64_t)now()});
	}else{
		auto itrper = perqtime.get();
		eosio_assert((uint64_t)now() - itrper.Time > ONEDAY,"Must have One day delay");
		itrper.Time = now();
		perqtime.set(itrper,_self);
	}
	int64_t id = cryptocertificate::get_next_certid();
	certu.emplace(_self,[&](auto &o){
		o.id = id;
		o.from = eosio::name{user};
		o.to = eosio::name{to};
		o.content = content;
		o.crtname = crtname;
		o.Time = (uint64_t)now();
	});
	

}

void cryptocertificate::ontransfer(
	account_name from,
	account_name to,
	eosio::extended_asset amount,
	std::string memo
){
//	eosio::print(from , " ", to ,"fuck!!!!!");
	
	cryptocertificate::crtr(from,to,"fuck","fuckyou",amount);
}
