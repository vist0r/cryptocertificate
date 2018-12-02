#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/singleton.hpp>
#include <string>
#include <vector>


class cryptocertificate : public eosio::contract{
		public :
			cryptocertificate(account_name self)
			:eosio::contract(self),
			whitelist(_self,_self),certid(_self,_self){}

			int qwhitelist(account_name user);

			eosio::asset get_unit_price();

			void adduser(account_name user,uint32_t role);

			void udestroy(account_name user);

			//@abi action
			void init();

			void crtrequest(
				account_name user,
				account_name to,
				std::string crtname,
				std::string content,
				eosio::extended_asset amt
			);

			//@abi action
			void ontransfer(
				account_name from ,
				account_name to, 
				eosio::asset amount, 
				std::string memo
			);

			uint64_t get_next_certid(){
				auto res = certid.get();
				res.id += 1;
				certid.set(res,_self);
				return res.id;

			}
		private :
			//@abi table udata
			struct userinfo{
				uint64_t id;
				uint32_t role;
				std::string uname;
				std::vector<std::string> cname;
				uint64_t primary_key() const{return id;}
				EOSLIB_SERIALIZE(userinfo,(id)(role)(uname)(cname))
			};

			//@abi table whitelist
			struct whiteList{
				uint64_t id;
				uint64_t primary_key() const{return id;}
				EOSLIB_SERIALIZE(whiteList,(id))
			};
			//@abi table cert
			struct certificaterequest{
				uint64_t id;
				std::string from;
				std::string to;
				std::string crtname;
				std::string content;
				time Time;
				uint64_t primary_key() const{return id;}
				EOSLIB_SERIALIZE(certificaterequest,(id)(from)(to)(crtname)(content))
			};
			
			//@abi table perqtime
			struct perrequest{
				time Time;
			};
			
			//@abi table certid
			struct certId{
				int64_t id;
			};

			typedef eosio::multi_index<N(udata),userinfo> userInfoIndex;

			typedef eosio::multi_index<N(whitelist),whiteList> whiteListIndex;
			whiteListIndex whitelist;

			typedef eosio::multi_index<N(cert),certificaterequest> certIndex;

			typedef eosio::singleton<N(perqtime),perrequest> singleton_pertime;

			typedef eosio::singleton<N(certid),certId> singleton_certid;
			singleton_certid certid;

};

//EOSIO_ABI(cryptocertificate,(ontransfer)(initwl))

extern "C"
{
	void apply(uint64_t receiver, uint64_t code,uint64_t action)
	{
		auto self = receiver;
		cryptocertificate thiscontract(self);
		if (code != receiver) return;
		switch(action){
			EOSIO_API(cryptocertificate,(ontransfer)(init))
		}
	}
}
