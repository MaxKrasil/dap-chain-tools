#include "dap_chain.h"
#include "dap_chain_mine.h"
#include "dap_common.h"
#include "dap_config.h"
#include "dap_enc.h"
#include "dap_enc_key.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
//#include <atomic>
//#include <cstdatomic>
/*#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>*/


#define LOG_TAG "dap-chain-cli"

//FILE* my_file_to_wite_gold;
FILE* my_file_to_wite_gold;
void* gold_mem;
FILE* my_file_to_wite_silver;
void* silver_mem;
FILE* my_file_to_wite_copper;
void* copper_mem;

int blocks_mined;
int blocks_mined_gold;
int blocks_mined_silver;
int blocks_mined_copper;

double total_mining_time;
double total_mining_hashes; //хз че делать пока
double total_hashes_in_minute;

size_t buf_enc_size;
size_t buf_size;
size_t buf_encrypted;
size_t buf_dec_size;

dap_enc_key_t* key;

void* buf_decrypted;
//std::atomic_flag work= ATOMIC_FLAG_INIT;
bool work;

void init() {
    dap_common_init(NULL);
    dap_config_init(NULL);
    dap_enc_init();
    dap_chain_init();
}

void print_help() {}

void make_new_block(){

}

void sing_handler(int signum){
    if (signum == 2){
    /*    munmap(gold_mem,16*blocks_mined);
        munmap(silver_mem, 16*blocks_mined);*/
        //fclose(my_file_to_wite_gold);
        //work.clear();
        dap_chain_mine_stop();
     /*   log_it(L_INFO, "Total hashes for gold coins %s B", ftell(my_file_to_wite_gold) );
        fclose(my_file_to_wite_gold);
        log_it(L_INFO, "Total hashes for silver coins %s B", ftell(my_file_to_wite_silver));
        fclose(my_file_to_wite_silver);
        log_it(L_INFO, "Total hashes for copper coins %s B", ftell(my_file_to_wite_copper));
        fclose(my_file_to_wite_copper);
        log_it(L_INFO, "Total blocks mined %s ", blocks_mined);
        log_it(L_INFO, "Gold blocks mined %s ", blocks_mined_gold);
        log_it(L_INFO, "Silver blocks mined %s ", blocks_mined_silver);
        log_it(L_INFO, "Copper blocks mined %s ", blocks_mined_copper);
        log_it(L_INFO, "Totla mining speed %s ", total_hashes_in_minute//blocks_mined);*/
        exit(0);
    }

}

int main(int argc, const char *argv[]) {
    log_it(L_INFO, "dap-chain-cli version 0.0.1 \n");
    signal(SIGINT, sing_handler);
    if (argc > 1) {
        init();
        char *l_config_name = strdup(argv[1]);
        dap_config_t *l_config = dap_config_open(l_config_name);
        if (l_config) {
            const char *l_chain_name =
                    dap_config_get_item_str(l_config, "general", "name");
            const char *l_chain_file_storage =
                    dap_config_get_item_str(l_config, "general", "file_storage");
            const char *l_chain_file_cache =
                    dap_config_get_item_str(l_config, "general", "file_cache");

            if (l_chain_name) {
                dap_chain_t *l_chain =
                        dap_chain_open(l_chain_file_storage, l_chain_file_cache);

                dap_chain_block_cache_t* l_g;
                dap_chain_block_cache_t* l_s;
                dap_chain_block_cache_t* l_c;
                dap_chain_file_open(l_g, l_s, l_c);
               // my_file_to_wite_gold = fopen("my_block_gold.txt", "w");// тестовый файл создам если есть добавим

             /*   if (my_file_to_wite_gold==NULL){
                    log_it(L_INFO, "Everything is lost! File not opened!");
                    exit(1);

                }


                my_file_to_wite_silver = fopen("my_block_silver.txt", "w");
                if (my_file_to_wite_silver==NULL){
                    log_it(L_INFO, "Everything is lost! File not opened!");
                    exit(1);

                }

                my_file_to_wite_copper = fopen("my_block_copper.txt", "w");
                if (my_file_to_wite_copper==NULL){
                    log_it(L_INFO, "Everything is lost! File not opened!");
                    exit(1);

                }*/


                if (l_chain) {
                    if (argc > 2) {
                        if (strcmp(argv[2], "encypt")==0){
                            if (argv[3]>  0){
                                if (argv[3] =="DAP_ENC_DATA_TYPE_MSRLN16"){
                                   // dap_enc_code();
                                }
                                else {
                                    log_it(L_CRITICAL, "Please, enter existing encryption type");
                                }
                            }
                            else {
                                log_it(L_CRITICAL, "Command encrypt need to be specified. "
                                                   "Please, enter encryption type");
                            }
                        }
                        else if (strcmp(argv[2], "decypt")==0){
                            if (argv[3]>  0){
                                if (argv[3] =="DAP_ENC_DATA_TYPE_MSRLN16"){
                                                                   // dap_enc_code();
                                                                }
                                                                else {
                                                                    log_it(L_CRITICAL, "Please, enter existing encryption type");
                                                                }
                            }
                            else {
                                log_it(L_CRITICAL, "Command decrypt need to be specified. "
                                                   "Please, enter encryption type");
                                }

                        }
                        else if (strcmp(argv[2], "block") == 0) {
                            if (argc > 3) {
                                if (strcmp(argv[3], "new") == 0) {//елси блон новый
                                    blocks_mined = 0;
                                    blocks_mined_copper = 0;
                                    blocks_mined_silver = 0;
                                    blocks_mined_gold = 0;
                                    total_hashes_in_minute = 0;
                                    log_it(L_INFO, "Create new block");//то пишем "блок новый"
                                    dap_chain_block_cache_t *l_block_cache;
                                    if (l_g==0&&l_s==0&&l_c==0){
                                    l_block_cache =

                                            dap_chain_allocate_next_block(l_chain);//размещаем новый блок
                                    }
                                    else if (l_g!=0){
                                        l_block_cache = l_g;
                                    }
                                    else if (l_s!=0){
                                        l_block_cache = l_s;
                                    }
                                    else if (l_c!=0){
                                        l_block_cache = l_g;
                                    }

                                    //m_hash_rate;
                                    if (dap_chain_mine_block(l_block_cache, false, 0) == 0) {//майним
                                        char *l_hash_str =
                                                dap_chain_hash_to_str_new(&l_block_cache->block_hash); //че-то хеш в строку переводим
                                        log_it(L_INFO, "Block mined with hash %s ", l_hash_str);

                                        dap_chain_count_new_block(l_block_cache);
                                        //total_hashes_in_minute = total_hashes_in_minute + sizeof(l_block_cache->block_hash)/l_block_cache->block_mine_time;
                                        //dap_chain_file_write(l_block_cache);
                                        dap_chain_update(l_block_cache);
//
                                        dap_chain_block_cache_dump(l_block_cache);//дамп че-то
                                        DAP_DELETE(l_hash_str);//хеш стирает

                                        while (true){
                                                dap_chain_block_new(&l_block_cache);
                                                l_block_cache =
                                                        dap_chain_allocate_next_block(l_chain);//размещаем новый блок
                                                if (dap_chain_mine_block(l_block_cache, false, 0) ==0){
                                                    blocks_mined+=1;
                                                        l_hash_str =
                                                             dap_chain_hash_to_str_new(&l_block_cache->block_hash); //че-то хеш в строку переводим
                                                         //dap_chain_file_write(l_block_cache);
                                                        dap_chain_update(l_block_cache);
//

                                                        log_it(L_INFO, "Block mined eith hash %s ", l_hash_str);
                                                        dap_chain_block_cache_dump(l_block_cache);//дамп че-то
                                                        DAP_DELETE(l_hash_str);//хеш стирает
                                                }

                                        }
                                    }
                                }
                            } else   {
                                log_it(L_CRITICAL, "Command 'block' need to be specified. "
                                                   "Variant: 'new' 'show' ");
                            }
                        } else if (strcmp(argv[2], "wallet") == 0) {
                            if (argc > 3) {
                                if (strcmp(argv[3], "create") == 0) {
                                    log_it(L_INFO, "Create blochain wallet");
                                }
                            } else {
                                log_it(L_CRITICAL, "Command 'wallet' need to be specified. "
                                                   "Variant: 'new' 'show' ");
                            }
                        }
                        /* encrypt and decrypt via cmd params */
						else if (strcmp(argv[2], "encrypt") == 0) {
							if (argc > 4) {
								/* What is supposed to be encrypted / decrypted ??? Let it be cmd param 4 */
								log_it(L_INFO, "Encryption with '%s' method", argv[3]);
								buf_size = strlen(argv[4]);
								buf_encrypted = (char *)calloc(1, buf_size * 4);
								/* Feistel */
								if (strcmp(argv[3], "FNAm2") == 0) {
//                                    key = dap_enc_key_new(64, DAP_ENC_KEY_TYPE_FNAM2);
                                    key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_FNAM2, 64);
                                    buf_enc_size = dap_enc_code(key, argv[4], buf_size, buf_encrypted, DAP_ENC_DATA_TYPE_RAW);
								}
								/* AES */
                                else if (strcmp(argv[3], "AES") ==0) {
                                    key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_AES, "SomeVeryLongString");
                                    //key = enc_key_create("SomeVeryLongString", DAP_ENC_KEY_TYPE_AES);
                                    buf_enc_size = dap_enc_code(key, argv[4], buf_size, buf_encrypted, DAP_ENC_DATA_TYPE_B64);
								}
                                else if (strcmp(argv[3], "MSRLN16") ==0){
                                    key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_RLWE_MSRLN16, 64);
                                  //Хз что тут писать:(//  buf_enc_size = dap_enc_code(key, argv[4], buf_size, buf_encrypted, DAP_ENC_DATA_TYPE_RLWE_MSRLN16);
                                }
							}
							else {
								log_it(L_CRITICAL, "Command 'encrypt' needs to be specified. "
													"Set encryption type. ");
							}
						}
						else if (strcmp(argv[2], "decrypt") == 0) {
							if (argc > 4) {
								log_it(L_INFO, "Decryption with '%s' method", argv[3]);
								if (strcmp(argv[3], "FNAm2") == 0) {
                                    buf_dec_size = dap_enc_decode(key, argv[4], buf_size, buf_decrypted, DAP_ENC_DATA_TYPE_RAW);
								}
								else if (strcmp(argv[3], "AES") == 0) {
                                    buf_dec_size = dap_enc_decode(key, argv[4], buf_size, buf_decrypted, DAP_ENC_DATA_TYPE_B64);
								}
                                else if (strcmp(argv[3], "MSRLN16")==0){
                                    //Пока хз что тут писать:(//buf_dec_size = dap_enc_decode(key, argv[4], buf_size, buf_decrypted, DAP_ENC_DATA_TYPE_RLWE_MSRLN16);
                                }
							}
							else {
								log_it(L_CRITICAL, "Command 'decrypt' needs to be specified. "
													"Set decryption type. ");
							}
						}
						/* Do smth with obtained buffers*/
						if (buf_encrypted) 
						{
							free(buf_encrypted);
                            buf_encrypted = 0;//nullptr;
						}
						if (buf_decrypted)
						{
							free(buf_decrypted);
                            buf_decrypted = 0;//nullptr;
						}
						/* */
                    } else {
                        log_it(L_INFO, "Information about '%s'", l_chain_name);
                        dap_chain_info_dump_log(l_chain);
                    }
                } else {
                    log_it(L_CRITICAL, "Can't open dapchain %s", l_chain_name);
                }
            } else {
                log_it(L_CRITICAL,
                       "Can't find dapchain name in config section 'general'");
            }
        } else {
            log_it(L_CRITICAL, "Can't open config name %s", l_config_name);
        }
        free(l_config_name);
    }
    return 0;
}
