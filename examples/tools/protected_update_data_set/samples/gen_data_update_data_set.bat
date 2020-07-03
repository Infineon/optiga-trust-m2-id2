set PATH=..\bin

:: Data update 
::RSA-SSA-PKCS1-V1_5-SHA-256 (RSA-1024)
::%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0E1 sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_1024_priv.pem payload_type=data offset=00 write_type=1 in_data_format="ascii" data=..\samples\payload\data\ascii_data.txt secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1

::RSA-SSA-PKCS1-V1_5-SHA-256 (RSA-2048)
::%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0E1 sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_2048_priv.pem payload_type=data offset=00 write_type=1 in_data_format="ascii" data=..\samples\payload\data\ascii_data.txt secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1

::RSA-SSA-PKCS1-V1_5-SHA-256 (RSA-1024) with hex file format
::%PATH%\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0E1 sign_algo=RSA-SSA-PKCS1-V1_5-SHA-256 priv_key=..\samples\integrity\sample_rsa_1024_priv.pem payload_type=data offset=00 write_type=1 in_data_format="hex" data=..\samples\payload\data\hex_data.txt secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1