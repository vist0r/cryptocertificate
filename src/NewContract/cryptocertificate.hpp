#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/transaction.hpp>
#include <string>
#include <vector>
using namespace eosio;

CONTRACT cryptocertificate : public contract{
		public :
			cryptocertificate( name receiver, name code, datastream<const char*> ds):contract(receiver,code,ds),
            whitelist(_self,_self.value),certid(_self,_self.value){}
	
			void apply(uint64_t receiver, uint64_t code, uint64_t action);  

			int qwhitelist(name user);

			uint64_t get_unit_price();

            uint64_t get_next_certid(){
				auto res = certid.get();
				res.id += 1;
				certid.set(res,_self);
				return res.id;
			}

            bool isuser(name user);
			
			ACTION adduser(name user,uint32_t role);

			ACTION udestroy(name user);

			ACTION init();
			
			ACTION crtr(
				name user,
				name to,
				std::string crtname,
				std::string content,
				eosio::extended_asset amt
			);

			ACTION ontransfer(
				name from ,
				name to, 
				eosio::extended_asset amount, 
				std::string memo
			);
			
			ACTION descrt(name to);
			
			ACTION addcert(name user,uint64_t id);


		private :
			TABLE userinfo{
				uint64_t id;
				uint32_t role;
				std::string uname;
				std::vector<std::string> cname;
				uint64_t primary_key() const{return id;}
				EOSLIB_SERIALIZE(userinfo,(id)(role)(uname)(cname))
			};

			TABLE whiteList{
				uint64_t id;
				uint64_t primary_key() const{return id;}
				EOSLIB_SERIALIZE(whiteList,(id))
			};

			TABLE certq{
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
			
			TABLE perq{
				uint64_t Time;
			};
			
			TABLE certId{
				int64_t id;
			};

		
			struct st_transfer{
				name from;
				name to;
				eosio::asset quantity;
				std::string memo;
			};

			typedef eosio::multi_index<"udata"_n,userinfo> userInfoIndex;

			typedef eosio::multi_index<"whitelist"_n,whiteList> whiteListIndex;
			whiteListIndex whitelist;

			typedef eosio::multi_index<"cert"_n,certq> certIndex;

			typedef eosio::singleton<"perqtime"_n,perq> singleton_pertime;

			typedef eosio::singleton<"certid"_n,certId> singleton_certid;
			singleton_certid certid;

};

//EOSIO_ABI(cryptocertificate,(ontransfer)(initwl))

void cryptocertificate::apply(uint64_t receiver, uint64_t code, uint64_t action) {
	auto &thiscontract = *this;
	if (name(code) == "eosio.token"_n && action == ("transfer"_n).value){
		auto transfer_data = eosio::unpack_action_data<st_transfer>();
		ontransfer(
			transfer_data.from,
			transfer_data.to,
			eosio::extended_asset(transfer_data.quantity,name(code)),
			transfer_data.memo
		);
		return;

	}
	if (code != get_self().value) return;
	switch (action) {
		EOSIO_DISPATCH_HELPER(
            cryptocertificate,
            (ontransfer)
            (init)
            (descrt)
            (adduser)
            (addcert)
            (udestroy)
        )
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
		cryptocertificate con(name(receiver),name(code),datastream<const char*>(nullptr,0));
		con.apply(receiver,code,action);
		eosio_exit(0);
	}
}
