# BEST PRICE
#### Video Demo:  <https://youtu.be/ufyY9zuyADg>
#### Description:
BEST PRICE is a app conected to two crypto exchanges: Binance and Coinbase. The objective of this app is to choose where is the best location to trade a crypto currency do this inside the app. BEST PRICE is connected to the exchanges's test plataform and is set up to 5 pairs of crypto. For Binance: BTC/USDT, ETH/USDT and BTC/ETH. For Coinbase: BTC/USD, ETH/USD and BTC/ETH.
#
## FIRST STEP:
First make sure you sigh up in the /register route. After you doing this step, you are ready to introduce your API keys in the database. Since BEST PRICE is made for testing plataforms, you must create your api keys based in [Binance's](https://testnet.binancefuture.com/en/futures/BTCUSDT) and [Coinbase's](https://public.sandbox.exchange.coinbase.com/) testing app.

## INTRODUCING API'S KEYS BINANCE:
In this stage you must have the Binance Testnet API Public Key and Secret Key. You need post both keys in the /index route.

## INTRODUCING API'S KEYS COINBASE:
For the Coinbase part is like the same procedure. You must have the Sandbox Coinbase API Public and Secret Keys plus the passphrase of this key.

## BUYING, SELLING and HISTORY:
After successfully completing the steps above you are ready to acess /buy, /sell and /history routes. To buy and sell, please make sure selecting the right pair of crypto currency, type a amount and press buy/sell to make your order. You order will show up at the /history route.
#
## REQUERIMENTS
Some imports and library is need such as [binance-pynthon](https://python-binance.readthedocs.io/en/latest/) and requests.
```
import json, hmac, hashlib, time, requests, base64
from requests.auth import AuthBase
from binance.client import Client as BClient
```
Since Binance got a python library, just need to get the api test keys and make a call using a var.
```
bclient = BClient(bapi_key, bapi_secret, testnet=True)
```
But for the Coinbase part was very hard to find out how to get the request done the right way. It was weeks of searching and study.
```
class CoinbaseExchangeAuth(AuthBase):
    def __init__(self, api_key, secret_key, passphrase):
        self.api_key = api_key
        self.secret_key = secret_key
        self.passphrase = passphrase

    def __call__(self, request):
        timestamp = str(time.time())
        message = timestamp + request.method + request.path_url + (request.body or b'').decode()
        hmac_key = base64.b64decode(self.secret_key)
        signature = hmac.new(hmac_key, message.encode(), hashlib.sha256)
        signature_b64 = base64.b64encode(signature.digest()).decode()

        request.headers.update({
            'CB-ACCESS-SIGN': signature_b64,
            'CB-ACCESS-TIMESTAMP': timestamp,
            'CB-ACCESS-KEY': self.api_key,
            'CB-ACCESS-PASSPHRASE': self.passphrase,
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        })
        return request

api_url = 'https://api-public.sandbox.pro.coinbase.com/'
```
Then just call the class
```
 auth = CoinbaseExchangeAuth(APIKEY, API_SECRET,  API_PASS)
```

