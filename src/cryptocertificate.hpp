#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/transaction.hpp>
#include <string>
#include <vector>

class cryptocertificate : public eosio::contract{
		public :
			cryptocertificate(account_name self)
			:eosio::contract(self),
			whitelist(_self,_self),certid(_self,_self){}
			void apply(account_name code,action_name action);

			int qwhitelist(account_name user);

			uint64_t get_unit_price();
			
			//@abi action
			void adduser(account_name user,uint32_t role);

			//@abi action
			void udestroy(account_name user);

			//@abi action
			void init();
			
			//@abi action
			void crtr(
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
				eosio::extended_asset amount, 
				std::string memo
			);

			uint64_t get_next_certid(){
				auto res = certid.get();
				res.id += 1;
				certid.set(res,_self);
				return res.id;

			}
			
			//@abi action
			void descrt(account_name to);

			bool isuser(account_name user);
			
			//@abi action
			void addcert(account_name user,uint64_t id);


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
			struct certq{
				uint64_t id;
				std::string from;
				std::string to;
				std::string crtname;
				std::string content;
				uint64_t times;
				uint64_t status;
				uint64_t primary_key() const{return id;}
				EOSLIB_SERIALIZE(certq,(id)(from)(to)(crtname)(content)(times)(status))
			};
			
			//@abi table perqtime
			struct perq{
				uint64_t Time;
			};
			
			//@abi table certid
			struct certId{
				int64_t id;
			};

		
			struct st_transfer{
				account_name from;
				account_name to;
				eosio::asset quantity;
				std::string memo;
			};

			typedef eosio::multi_index<N(udata),userinfo> userInfoIndex;

			typedef eosio::multi_index<N(whitelist),whiteList> whiteListIndex;
			whiteListIndex whitelist;

			typedef eosio::multi_index<N(cert),certq> certIndex;

			typedef eosio::singleton<N(perqtime),perq> singleton_pertime;

			typedef eosio::singleton<N(certid),certId> singleton_certid;
			singleton_certid certid;

};

//EOSIO_ABI(cryptocertificate,(ontransfer)(initwl))

void cryptocertificate::apply(account_name code,account_name action){
	auto &thiscontract = *this;
	if (code == N(eosio.token) && action == N(transfer)){
		auto transfer_data = eosio::unpack_action_data<st_transfer>();
		ontransfer(
			transfer_data.from,
			transfer_data.to,
			eosio::extended_asset(transfer_data.quantity,code),
			transfer_data.memo
		);
		return;

	}
	if (code != _self) return;
	switch (action) {
		EOSIO_API(cryptocertificate,(ontransfer)(init)(descrt)(adduser)(addcert)(udestroy))
	};

}

extern "C"
{
	void apply(uint64_t receiver, uint64_t code,uint64_t action)
	{
		/*
		auto self = receiver;
		cryptocertificate thiscon(self);
		if (code == N(eosio.token) && action == N(transfer)){
			execute_action(&thiscon,&cryptocertificate::ontransfer);
			return;
		}
		cryptocertificate thiscontract(self);
		if (code != receiver) return;
		switch(action){
			EOSIO_API(cryptocertificate,(ontransfer)(init)(crtr))
		}
		*/
		cryptocertificate con(receiver);
		con.apply(code,action);
		eosio_exit(0);
	}
}
