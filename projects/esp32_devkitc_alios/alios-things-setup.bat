:: Download the AliOS-Things git repo
git clone https://github.com/alibaba/AliOS-Things.git -b rel_2.1.0
:: Entering into AliOS-Things directory
cd AliOS-Things
:: Apply the patch which contain the optiga shell application related changes 
git apply --whitespace=fix ../alios-2.1.0-optiga-shell-application.patch
:: Copy relevant OPTIGA host library into respective location of AliOS-Things framework
xcopy ..\..\..\examples 3rdparty\experimental\optiga\examples /E /I
xcopy ..\..\..\externals 3rdparty\experimental\optiga\externals /E /I
xcopy ..\..\..\optiga 3rdparty\experimental\optiga\optiga /E /I
xcopy ..\..\..\pal\esp32_devkitc_alios 3rdparty\experimental\optiga\pal\esp32_devkitc_alios /E /I
copy ..\..\..\pal\pal_crypt_mbedtls.c 3rdparty\experimental\optiga\pal\pal_crypt_mbedtls.c
copy ..\..\..\externals\alios\security\irot\se\src\core\optiga_se_adapter.c security\irot\se\src\core\
copy ..\..\..\examples\optiga\usecases\example_optiga_hmac_verify_with_authorization_reference.c 3rdparty\experimental\optiga\examples\optiga\
copy ..\..\..\examples\optiga\usecases\example_pair_host_and_optiga_using_pre_shared_secret.c 3rdparty\experimental\optiga\examples\optiga\
del 3rdparty\experimental\optiga\examples\optiga\usecases\example_optiga_hmac_verify_with_authorization_reference.c
del 3rdparty\experimental\optiga\examples\optiga\usecases\example_pair_host_and_optiga_using_pre_shared_secret.c