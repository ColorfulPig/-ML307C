
ETHIRDPARTY_WOLFSSL_DIR := third-party/wolfssl



# OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/src/internal.c
# OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/src/keys.c
# OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/src/ssl.c
# OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/src/tls.c
# OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/src/wolfio.c
# OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/aes.c
# OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/asn.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/coding.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/hash.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/hmac.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/integer.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/md5.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/misc.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/random.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/rsa.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/sha.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/sha256.c
OC_FILES +=  $(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3/wolfcrypt/src/wc_port.c

INC      += -I'$(ETHIRDPARTY_WOLFSSL_DIR)/port'
INC      += -I'$(ETHIRDPARTY_WOLFSSL_DIR)/wolfssl-3.15.3'