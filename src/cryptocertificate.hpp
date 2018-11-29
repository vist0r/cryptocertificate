#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>
#include <vector>
class cryptocertificate : public eosio::contract{
		public :
			cryptocertificate(account_name self)
			:eosio::contract(self),
			data(_self,_self),
			whitelist(_self,_self){}

			int qwhitelist(account_name user);

			void adduser(account_name user,uint32_t role);

			void udestroy(account_name user);

			void initwl();

			void crtcert(
				account_name user,
				std::string crtname,
				std::string from ,
				std::string to,
				std::string content
			);
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

			typedef eosio::multi_index<N(udata),userinfo> userInfoIndex;
			userInfoIndex data;

			typedef eosio::multi_index<N(whitelist),whiteList> whiteListIndex;
			whiteListIndex whitelist;

};

EOSIO_ABI(cryptocertificate,(adduser)(udestroy)(initwl))
