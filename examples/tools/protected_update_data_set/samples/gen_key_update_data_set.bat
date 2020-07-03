set PATH=..\bin

:: Key update 

:: RSA-1024 key update
%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0FC sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_1024_priv.pem payload_type=key key_algo=65 key_usage=12 key_data=..\samples\payload\key\rsa_1024_test.pem secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1

:: RSA-2048 key update
::%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0FD sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_2048_priv.pem payload_type=key key_algo=66 key_usage=10 key_data=..\samples\payload\key\rsa_2048_test.pem

:: AES-128 key update
::%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E200 sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_1024_priv.pem payload_type=key key_algo=129 key_usage=02 key_data=..\samples\payload\key\aes_key_128.txt secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1

:: AES-192 key update
::%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E200 sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_1024_priv.pem payload_type=key key_algo=130 key_usage=02 key_data=..\samples\payload\key\aes_key_192.txt secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1

:: AES-256 key update
::%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E200 sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_1024_priv.pem payload_type=key key_algo=131 key_usage=02 key_data=..\samples\payload\key\aes_key_256.txt secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1

