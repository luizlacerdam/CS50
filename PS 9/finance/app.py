import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import date, datetime
# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "GET":
        rows = db.execute("SELECT symbol, share_balance FROM bank WHERE id = ?", session["user_id"])
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        # cash = cash[0]
        # cash = cash["cash"]
        sumer = 0

        return render_template("index.html", rows=rows, cash=cash, lookup=lookup, round=round, sumer=sumer, sum=sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":

        person_id = session["user_id"]
        tran_type = "buy"
        data = lookup(request.form.get("symbol"))

        if data == None:
            return apology("Wrong Symbol")
        if data == "":
            return apology("Symbol is missing")

        symbol = data["symbol"]
        shares = request.form.get("shares")

        if shares == None or shares == "":
            return apology("shares is missing")

        if shares.isnumeric() == False:
            return apology("Only numbers")

        price = data["price"]
        now = datetime.now()
        current_time = now.strftime("%H:%M:%S")
        todayis = date.today()

        if person_id == None or symbol == None or shares == None or shares.isnumeric() == False or price == None or todayis == None or current_time == None:
            return apology("Something went wrong")
        else:

            total = price * int(shares)

            # current cash, check if enought cash
            bcash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

            if bcash[0]["cash"] < total:
                return apology("NOT ENOUGHT CASH", 403)

            # new cash
            ncash = bcash[0]["cash"] - total
            # update new cash
            db.execute("UPDATE users SET cash = ? WHERE id = ?", ncash, session["user_id"])

            # bank update
            data_bank = db.execute("SELECT share_balance FROM bank WHERE id = ? AND symbol = ?", person_id, symbol)
            if len(data_bank) != 1:
                db.execute("INSERT INTO bank (id, symbol, share_balance) VALUES(?, ?, ?)", person_id, symbol, shares)

            if len(data_bank) == 1:
                db.execute("UPDATE bank SET share_balance = ? WHERE id = ? AND symbol = ?", shares, person_id, symbol)

            # buy the shares
            db.execute("INSERT INTO transactions (id, type, symbol, shares, price, time, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
                       person_id, tran_type, symbol, shares, price, current_time, todayis)
            return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    data = db.execute("SELECT symbol, shares, price, date, time, type FROM transactions WHERE id = ?", session["user_id"])

    return render_template("history.html", data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        username = request.form.get("username")
        user_indb = username.lower()
        rows = db.execute("SELECT * FROM users WHERE username = ?", user_indb)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote_lu = lookup(symbol)
        if symbol == None or symbol == "" or len(symbol) == 0:
            return apology("Missing symbol", 400)
        if quote_lu == None:
            return apology("Invalid Symbol")

        price = quote_lu["price"]

        return render_template("quoted.html", quote_lu=quote_lu, price=price)


@app.route("/register", methods=["GET", "POST"])
def register():

    input = request.form.get("username")
    password = request.form.get("password")
    passwordconfirm = request.form.get("confirmation")

    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        username = input.lower()
        username_indb = len(db.execute("SELECT username FROM users WHERE username = ?", username))

        if (password != passwordconfirm):
            return apology("Passwords are diferrent", 400)
        if len(password) < 6:
            return apology("Password must have more then six chars long", 400)
        if username == None or len(username) > 32 or username == "":
            return apology("Type an username lower then 32 chars", 400)
        if username_indb > 0:
            return apology("Username already register", 400)
        if len(username) <= 32 and len(username) >= 4 and len(password) >= 6 and password == passwordconfirm and username_indb == 0:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))
            return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        data = db.execute("SELECT symbol, share_balance FROM bank WHERE id = ?", session["user_id"])

        return render_template("sell.html", data=data)

    if request.method == "POST":

        symbol = request.form.get("symbol")
        if symbol == None:
            return apology("THERE IS NO SYMBOL", 403)

        shares = int(request.form.get("shares"))
        if shares == "" or shares < 1 or shares == 0:
            return apology("Missing shares", 400)

        shares_data = db.execute("SELECT share_balance FROM bank WHERE id = ? AND symbol = ?", session["user_id"], symbol)

        if shares > shares_data[0]["share_balance"]:
            return apology("YOU DONT OWN HAVE THIS MANY SHARES", 400)

        person_id = session["user_id"]
        tran_type = "sell"
        data = lookup(symbol)
        price = data["price"]
        now = datetime.now()
        current_time = now.strftime("%H:%M:%S")
        todayis = date.today()

        # update cash
        bcash = db.execute("SELECT cash from users WHERE id = ?", person_id)

        total = price * shares

        ncash = bcash[0]["cash"] + total

        db.execute("UPDATE users SET cash = ? WHERE id = ?", ncash, person_id)

        # registe transaction
        db.execute("INSERT INTO transactions (id, type, symbol, shares, price, time, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   person_id, tran_type, symbol, shares, price, current_time, todayis)

        # update balance
        new_shares_balance = shares_data[0]["share_balance"] - shares
        db.execute("UPDATE bank SET share_balance = ? WHERE id = ? AND symbol = ?", new_shares_balance, person_id, symbol)

        return redirect("/")