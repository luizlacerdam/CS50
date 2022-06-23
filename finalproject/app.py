import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from helpers import login_required, lookup, usd, check, on_message, on_error, on_close, on_open, crypto
##########################################################
import json, hmac, hashlib, time, requests, base64
from requests.auth import AuthBase
from binance.client import Client as BClient
###########################################################
# Configure application
app = Flask(__name__)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///exchange.db")

# Custom filter
app.jinja_env.filters["usd"] = usd


#Binance client general var
# binancepubkey = db.execute("SELECT binance_key FROM users WHERE id = ?", session["user_id"])
# binanceseckey = db.execute("SELECT binance_secret FROM users WHERE id = ?", session["user_id"])
# bapi_key = binancepubkey[0]["binance_key"]
# bapi_secret = binanceseckey[0]["binance_secret"]
bapi_key = None
bapi_secret = None


##########################
# Create custom authentication for Exchange
APIKEY = None
API_SECRET = None
API_PASS = None
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



@app.route("/", methods=["GET", "POST"])
def home():

    if request.method == "GET":
        return render_template("home.html")

    if request.method == "POST":
        return render_template("home.html")

@app.route("/index", methods=["GET","POST"])
@login_required
def index():

    if request.method == "GET":
        binancepub = db.execute("SELECT binance_key FROM users WHERE id = ?", session["user_id"])
        coinbasepub = db.execute("SELECT coinbase_key FROM users WHERE id = ?", session["user_id"])
        # both setup
        if binancepub[0]["binance_key"] != None and coinbasepub[0]["coinbase_key"] != None:
            resultb = binancepub[0]["binance_key"]
            resultc = coinbasepub[0]["coinbase_key"]
            resultbsec = "Your secret key is crypted!"
            resultcsec = "Your secret key is crypted!"
            resultcpass = "Your passphrase is crypted!"
            bar = "100"
            text = "YOU ARE READY TO TRADE!"
            return render_template("index.html", resultb = resultb, resultc = resultc, resultbsec = resultbsec, resultcsec = resultcsec, resultcpass=resultcpass, bar=bar, text=text)
        # binance setup
        if binancepub[0]["binance_key"] != None and coinbasepub[0]["coinbase_key"] == None:
            resultb = binancepub[0]["binance_key"]
            resultbsec = "Your secret key is crypted!"
            resultc = "Setup you public key!"
            resultcsec = "Setup you secret key!"
            resultcpass = "Setup you passphrase!"
            bar = "50"
            text = "SETUP YOUR KEYS BEFORE USING BUY, SELL AND HISTORY!"
            return render_template("index.html", resultb = resultb, resultc = resultc, resultbsec = resultbsec, resultcsec = resultcsec, resultcpass=resultcpass, bar=bar, text=text)
        # coinbase setup
        if binancepub[0]["binance_key"] == None and coinbasepub[0]["coinbase_key"] != None:
            resultb = "Setup you public key!"
            resultbsec = "Setup you secret key!"
            resultc = coinbasepub[0]["coinbase_key"]
            resultcsec = "Your secret key is crypted!"
            resultcpass = "Your passphrase is crypted!"
            bar = "50"
            text = "SETUP YOUR KEYS BEFORE USING BUY, SELL AND HISTORY!"
            return render_template("index.html", resultb = resultb, resultc = resultc, resultbsec = resultbsec, resultcsec = resultcsec, resultcpass=resultcpass, bar=bar, text=text)
         #both null
        if binancepub[0]["binance_key"] == None and coinbasepub[0]["coinbase_key"] == None:
            resultb = "Setup you public key!"
            resultbsec = "Setup you secret key!"
            resultc = "Setup you public key!"
            resultcsec = "Setup you secret key!"
            resultcpass = "Setup you passphrase!"
            bar = "0"
            text = "SETUP YOUR KEYS BEFORE USING BUY, SELL AND HISTORY!"
            return render_template("index.html", resultb = resultb, resultc = resultc, resultbsec = resultbsec, resultcsec = resultcsec, resultcpass=resultcpass, bar=bar, text=text)

    if request.method == "POST":
        binancepub = request.form.get("binance-public")
        binancesec = request.form.get("binance-secret")
        coinbasepub = request.form.get("coinbase-public")
        coinbasesec = request.form.get("coinbase-secret")
        coinbasepass = request.form.get("coinbase-pass")

        if binancepub != None and binancesec != None:
            db.execute("UPDATE users SET binance_key = ?,binance_secret = ? WHERE id = ?", binancepub, binancesec, session["user_id"])
        elif coinbasepub != None and coinbasesec != None and coinbasepass != None:
            db.execute("UPDATE users SET coinbase_key = ?, coinbase_secret = ?, coinbase_pass = ? WHERE id = ?", coinbasepub, coinbasesec, coinbasepass, session["user_id"])
        return redirect("/index")

@app.route("/register", methods=["GET", "POST"])
def register():
    input = request.form.get("email")
    password = request.form.get("password")
    passconfirmation = request.form.get("confirmation")

    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":

        email = input.lower()
        email_indb = db.execute("SELECT email FROM users WHERE email = ?", email)

        if len(email_indb) > 0:
            flash("Email adress already register!")
            return redirect(request.url)

        if check(email) == False:
            flash("Invalid Email")
            return redirect(request.url)

        if password != passconfirmation:
            flash("Passwords must match!")
            return redirect(request.url)

        if len(password) < 6 or password == "":
            flash("Password at least 6 chars!")
            return redirect(request.url)
        if check(email) == True and len(password) > 5 and len(email_indb) == 0:

            db.execute("INSERT INTO users (email, hash) VALUES (?, ?)", email, generate_password_hash(password))
            return redirect("/login")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    if session == True:
        # Forget any user_id
        session.clear()

    if request.method == "GET":
        return render_template("login.html")

    if request.method == "POST":
        email = request.form.get("email")
        password = request.form.get("password")

        # Ensure username was submitted
        if email == "" or len(email) == 0:
            flash("Must provide a email adress!")
            return redirect(request.url)

        # Ensure password was submitted
        if  password == 0 or password == "":
            flash("Must provide a password!")
            return redirect(request.url)

        # Query database for username
        email_indb = email.lower()
        rows = db.execute("SELECT * FROM users WHERE email = ?", email_indb)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            flash("Invalid email and/or password!")
            return redirect(request.url)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/index")

@app.route("/logout")
@login_required
def logout():

    session.clear()

    return redirect("/")

@app.route("/buy", methods=["GET","POST"])
@login_required
def buy():
    # GET BINANCE API KEYS FROM DB
    binancepubkey = db.execute("SELECT binance_key FROM users WHERE id = ?", session["user_id"])
    binanceseckey = db.execute("SELECT binance_secret FROM users WHERE id = ?", session["user_id"])
    bapi_key = binancepubkey[0]["binance_key"]
    bapi_secret = binanceseckey[0]["binance_secret"]
    # MAKES BINANCE CLIENT
    bclient = BClient(bapi_key, bapi_secret, testnet=True)
    #GET COINBASE KEYS FROM DB
    coinbasepubkey = db.execute("SELECT coinbase_key FROM users WHERE id = ?", session["user_id"])
    coinbaseseckey = db.execute("SELECT coinbase_secret FROM users WHERE id = ?", session["user_id"])
    coinbasepass = db.execute("SELECT coinbase_pass FROM users WHERE id = ?", session["user_id"])

    APIKEY = coinbasepubkey[0]["coinbase_key"]
    API_SECRET = coinbaseseckey[0]["coinbase_secret"]
    API_PASS = coinbasepass[0]["coinbase_pass"]
    # MAKES COINBASE CONNECTION
    auth = CoinbaseExchangeAuth(APIKEY, API_SECRET,  API_PASS)

    if request.method == "GET":
        if bapi_key == None or APIKEY == None or bapi_secret == None or API_SECRET == None or API_PASS == None:
            flash("Set up your keys")
            return redirect("/index")
        # BINANCE GET CLIENT ACCOUNT FOR BALANCES
        account_spot = bclient.get_account()
        # COINBASE ACCOUNT FOR BALANCES
        coinbase_acc_response = requests.get(api_url + 'accounts', auth=auth)
        account_coinbase = coinbase_acc_response.json()

        return render_template("buy.html", account_spot=account_spot, float=float, account_coinbase=account_coinbase, crypto=crypto)



@app.route("/buy-binance", methods=["GET","POST"])
@login_required
def buybinance():
    # GET BINANCE API KEYS FROM DB
    binancepubkey = db.execute("SELECT binance_key FROM users WHERE id = ?", session["user_id"])
    binanceseckey = db.execute("SELECT binance_secret FROM users WHERE id = ?", session["user_id"])
    bapi_key = binancepubkey[0]["binance_key"]
    bapi_secret = binanceseckey[0]["binance_secret"]
    # MAKES BINANCE CLIENT
    bclient = BClient(bapi_key, bapi_secret, testnet=True)

    if request.method == "GET":
        redirect("/buy")

    if request.method == "POST":

        # BINANCE CREAT ORDER
        binance_symbol = request.form.get("binance-symbol")
        binance_amount = request.form.get("binance-amount")

        if binance_symbol == "BTC/USDT":
            binance_symbol = "BTCUSDT"
        if binance_symbol == "ETH/USDT":
            binance_symbol = "ETHUSDT"
        if binance_symbol == "ETH/BTC":
            binance_symbol = "ETHBTC"

        order = bclient.order_market_buy(
        symbol=binance_symbol,
        quantity=binance_amount)

        return redirect("/buy")

@app.route("/buy-coinbase", methods=["GET","POST"])
@login_required
def buycoinbase():
    #GET COINBASE KEYS FROM DB
    coinbasepubkey = db.execute("SELECT coinbase_key FROM users WHERE id = ?", session["user_id"])
    coinbaseseckey = db.execute("SELECT coinbase_secret FROM users WHERE id = ?", session["user_id"])
    coinbasepass = db.execute("SELECT coinbase_pass FROM users WHERE id = ?", session["user_id"])

    APIKEY = coinbasepubkey[0]["coinbase_key"]
    API_SECRET = coinbaseseckey[0]["coinbase_secret"]
    API_PASS = coinbasepass[0]["coinbase_pass"]
    # MAKES COINBASE CONNECTION
    auth = CoinbaseExchangeAuth(APIKEY, API_SECRET,  API_PASS)

    if request.method == "GET":
        redirect("/buy")

    if request.method == "POST":
        coinbase_symbol = request.form.get("coinbase-symbol")
        coinbase_amount = request.form.get("coinbase-amount")
        coinbase_price = request.form.get("coinbase-price")

        if coinbase_symbol == "BTC/USD":
            coinbase_symbol = "BTC-USD"
        if coinbase_symbol == "ETH/USD":
            coinbase_symbol = "ETH-USD"
        if coinbase_symbol == "ETH/BTC":
            coinbase_symbol = "ETH-BTC"

        # PLACE MARKET ORDER
        order = {
            'type': "limit",
            'size': coinbase_amount,
            'price': coinbase_price,
            'side': 'buy',
            'product_id': coinbase_symbol,
            "post_only": False
        }
        coinbase_order = requests.post(api_url + 'orders', json=order, auth=auth)
        coinbase_order_response = coinbase_order.json()
        # print(coinbase_amount)
        # print(coinbase_symbol)
        print(coinbase_order_response)
        # print(coinbase_price)
        return redirect("/buy")

@app.route("/sell", methods=["GET","POST"])
@login_required
def sell():
    # GET BINANCE API KEYS FROM DB
    binancepubkey = db.execute("SELECT binance_key FROM users WHERE id = ?", session["user_id"])
    binanceseckey = db.execute("SELECT binance_secret FROM users WHERE id = ?", session["user_id"])
    bapi_key = binancepubkey[0]["binance_key"]
    bapi_secret = binanceseckey[0]["binance_secret"]
    # MAKES BINANCE CLIENT
    bclient = BClient(bapi_key, bapi_secret, testnet=True)
    #GET COINBASE KEYS FROM DB
    coinbasepubkey = db.execute("SELECT coinbase_key FROM users WHERE id = ?", session["user_id"])
    coinbaseseckey = db.execute("SELECT coinbase_secret FROM users WHERE id = ?", session["user_id"])
    coinbasepass = db.execute("SELECT coinbase_pass FROM users WHERE id = ?", session["user_id"])

    APIKEY = coinbasepubkey[0]["coinbase_key"]
    API_SECRET = coinbaseseckey[0]["coinbase_secret"]
    API_PASS = coinbasepass[0]["coinbase_pass"]
    # MAKES COINBASE CONNECTION
    auth = CoinbaseExchangeAuth(APIKEY, API_SECRET,  API_PASS)

    if request.method == "GET":
        if bapi_key == None or APIKEY == None or bapi_secret == None or API_SECRET == None or API_PASS == None:
            flash("Set up your keys")
            return redirect("/index")
        # BINANCE GET CLIENT ACCOUNT FOR BALANCES
        account_spot = bclient.get_account()
        # COINBASE ACCOUNT FOR BALANCES
        coinbase_acc_response = requests.get(api_url + 'accounts', auth=auth)
        account_coinbase = coinbase_acc_response.json()

        return render_template("sell.html", account_spot=account_spot, float=float, account_coinbase=account_coinbase, crypto=crypto)

@app.route("/sell-binance", methods=["GET","POST"])
@login_required
def sellbinance():
    binancepubkey = db.execute("SELECT binance_key FROM users WHERE id = ?", session["user_id"])
    binanceseckey = db.execute("SELECT binance_secret FROM users WHERE id = ?", session["user_id"])
    bapi_key = binancepubkey[0]["binance_key"]
    bapi_secret = binanceseckey[0]["binance_secret"]
    bclient = BClient(bapi_key, bapi_secret, testnet=True)
    if request.method == "GET":
        return redirect("/sell")
    if request.method == "POST":
        binance_symbol = request.form.get("binance-symbol")
        amount = request.form.get("binance-amount")

        if binance_symbol == "BTC/USDT":
            binance_symbol = "BTCUSDT"
        if binance_symbol == "ETH/USDT":
            binance_symbol = "ETHUSDT"
        if binance_symbol == "ETH/BTC":
            binance_symbol = "ETHBTC"

        order = bclient.order_market_sell(
        symbol=binance_symbol,
        quantity=amount)
        return redirect("/sell")
@app.route("/sell-coinbase", methods=["GET","POST"])
@login_required
def sellcoinbase():
    #GET COINBASE KEYS FROM DB
    coinbasepubkey = db.execute("SELECT coinbase_key FROM users WHERE id = ?", session["user_id"])
    coinbaseseckey = db.execute("SELECT coinbase_secret FROM users WHERE id = ?", session["user_id"])
    coinbasepass = db.execute("SELECT coinbase_pass FROM users WHERE id = ?", session["user_id"])

    APIKEY = coinbasepubkey[0]["coinbase_key"]
    API_SECRET = coinbaseseckey[0]["coinbase_secret"]
    API_PASS = coinbasepass[0]["coinbase_pass"]
    # MAKES COINBASE CONNECTION
    auth = CoinbaseExchangeAuth(APIKEY, API_SECRET,  API_PASS)

    if request.method == "GET":
        redirect("/sell")

    if request.method == "POST":
        coinbase_symbol = request.form.get("coinbase-symbol")
        coinbase_amount = request.form.get("coinbase-amount")
        coinbase_price = request.form.get("coinbase-price")

        if coinbase_symbol == "BTC/USD":
            coinbase_symbol = "BTC-USD"
        if coinbase_symbol == "ETH/USD":
            coinbase_symbol = "ETH-USD"
        if coinbase_symbol == "ETH/BTC":
            coinbase_symbol = "ETH-BTC"

        # PLACE MARKET ORDER
        order = {
            'type': "limit",
            'size': coinbase_amount,
            'price': coinbase_price,
            'side': 'sell',
            'product_id': coinbase_symbol,
            "post_only": False
        }
        coinbase_order = requests.post(api_url + 'orders', json=order, auth=auth)
        coinbase_order_response = coinbase_order.json()
        print(coinbase_order_response)
        return redirect("/sell")

@app.route("/history", methods=["GET","POST"])
@login_required
def history():
    binancepubkey = db.execute("SELECT binance_key FROM users WHERE id = ?", session["user_id"])
    binanceseckey = db.execute("SELECT binance_secret FROM users WHERE id = ?", session["user_id"])
    bapi_key = binancepubkey[0]["binance_key"]
    bapi_secret = binanceseckey[0]["binance_secret"]
    bclient = BClient(bapi_key, bapi_secret, testnet=True)

     #GET COINBASE KEYS FROM DB
    coinbasepubkey = db.execute("SELECT coinbase_key FROM users WHERE id = ?", session["user_id"])
    coinbaseseckey = db.execute("SELECT coinbase_secret FROM users WHERE id = ?", session["user_id"])
    coinbasepass = db.execute("SELECT coinbase_pass FROM users WHERE id = ?", session["user_id"])

    APIKEY = coinbasepubkey[0]["coinbase_key"]
    API_SECRET = coinbaseseckey[0]["coinbase_secret"]
    API_PASS = coinbasepass[0]["coinbase_pass"]
    # MAKES COINBASE CONNECTION
    auth = CoinbaseExchangeAuth(APIKEY, API_SECRET,  API_PASS)
    # CHECK IF API KEYS IS SETUP
    if request.method == "GET":
        if bapi_key == None or APIKEY == None or bapi_secret == None or API_SECRET == None or API_PASS == None:
            flash("Set up your keys")
            return redirect("/index")
        # BINANCE TRADES
        account_spot = bclient.get_account()
        trade = bclient.get_my_trades(symbol='BTCUSDT')
        trades = reversed(trade)
        # COINBASE TRADES
        orders = {
        'limit': 100,
        'status': "all"
        }
        get_all_orders = requests.get(api_url + 'orders',json=orders, auth=auth)
        coinbase_orders = get_all_orders.json()
        return render_template("history.html", account_spot=account_spot, float=float, trades=trades,len=len, coinbase_orders=coinbase_orders)


