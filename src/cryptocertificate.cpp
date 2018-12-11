#include "cryptocertificate.hpp"

#define EOS_SYMBOL S(4,EOS)

const time ONEDAY = 24 * 3600;
bool cryptocertificate :: isuser(account_name user){
	userInfoIndex data(_self,user);
	return !(data.begin() == data.end());
}
std::string account_to_string(account_name user){
	auto namecheck = eosio::name{user};
	return namecheck.to_string();
}
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
	eosio::extended_asset amt)
{	
	eosio_assert(isuser(user),"Must in userList!");
	eosio_assert(cryptocertificate::get_unit_price() * content.length() == amt.amount,"Invaild amount!");
	eosio_assert(amt.contract == N(eosio.token), "Only true EOS token is allowed!");
	eosio_assert(amt.symbol == EOS_SYMBOL,"Only true EOS token is allowed!");
	certIndex certu(_self,user);
	certIndex certt(_self,to);
	singleton_pertime perqtime(_self,user);
	if (!perqtime.exists()){
		auto itrper = perqtime.get_or_create(_self,perq{(uint64_t)now()});
	}else{
		auto itrper = perqtime.get();
//		eosio_assert((uint64_t)now() - itrper.Time > ONEDAY,"Must have One day delay");
		itrper.Time = now();
		perqtime.set(itrper,_self);
	}

	int64_t id = cryptocertificate::get_next_certid();
	certu.emplace(_self,[&](auto &o){
		o.id = id;
		o.from = account_to_string(user);
		o.to = account_to_string(to);
		o.content = content;
		o.crtname = crtname;
		o.times = (uint64_t)now();
		o.status = 0; 
	});
	certt.emplace(_self,[&](auto &o){
		o.id = id;
		o.from  = account_to_string(user);
		o.to = account_to_string(to);
		o.content = content;
		o.times = (uint64_t)now();
		o.status = 0;
	});

	
}
void cryptocertificate::ontransfer(
	account_name from,
	account_name to,
	eosio::extended_asset amount,
	std::string memo
){
	
	cryptocertificate::crtr(from,to,"fuck","f",amount);
}

void cryptocertificate::descrt(account_name to){
	certIndex certu(_self,to);
	while (certu.begin() != certu.end()){
		certu.erase(certu.begin());	
	}
	singleton_pertime prt(_self,to);
	prt.remove();

}

void cryptocertificate::addcert(account_name user,uint64_t id){
	userInfoIndex udata(_self,user);
	certIndex certu(_self,user);
	auto itr = udata.find(account_to_uint64(user));
	auto itrn = certu.find(id);
	eosio_assert(itrn != certu.end(),"Not find certificate!");
	eosio_assert(itr->uname == account_to_string(user),"Not find user!");
	udata.modify(itr,_self,[&](auto &o){
		o.cname.push_back(itrn->crtname);
	});
}
