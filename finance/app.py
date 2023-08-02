import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

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
    #transactions = db.execute("SELECT * FROM transactions WHERE username = ?", session["username"])
    transactions = db.execute("SELECT symbol, name, SUM(shares) FROM transactions WHERE username = ? GROUP BY symbol HAVING SUM(shares)>0", session["username"])
    total_total = 0.0
    for transaction in transactions:
        #adds current price to dictionary
        transaction["current_price"] = lookup(transaction["symbol"])["price"]
        #multiplies number of shares by current price to get current value of stocks held
        transaction["total"] = transaction["SUM(shares)"] * transaction["current_price"]
        #adds the values of stock together
        total_total += transaction["total"]
    user_cash = db.execute("SELECT cash FROM users WHERE username = ?", session["username"])[0]["cash"]
    #adds user cash to total total
    total_total += user_cash
    page_status = session["page_status"]
    #resets page status to none
    session["page_status"] = "none"
    return render_template("index.html", page_status=page_status, transactions=transactions,total_total=total_total,user_cash=user_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        #checks if symbol was submitted
        if not request.form.get("symbol"):
            #if symbol was blank
            return apology("must provide stock symbol", 400)

        #checks fractional shares
        try:
            int(request.form.get("shares"))
        except ValueError:
            return apology("number of shares must be a positive integer", 400)

        #checks if number of shares is not a positive number (e.g., blank)
        if not int(request.form.get("shares")) > 0:
            return apology("must provide number of shares", 400)

        #looks up stock and returns dict with keys name, price, symbol
        stock_dict = lookup(request.form.get("symbol"))
        #checks if stock was found during lookup
        #stock in db is stock symbol
        if (stock_dict):
            #checks user cash
            user_cash = db.execute("SELECT cash FROM users WHERE username = ?", session["username"])[0]["cash"]
            #cost of stock
            cost_stock = float(request.form.get("shares")) * float(stock_dict["price"])
            #checks if user has enough cash for purchase
            if (cost_stock < user_cash):
                db.execute("INSERT INTO transactions(username, symbol, name, shares, price, time) VALUES(?,?,?,?,?,?);", session["username"], stock_dict["symbol"], stock_dict["name"], request.form.get("shares"), stock_dict["price"], str(datetime.utcnow())[0:-7])

                #subtracts cost of stock from user cash
                user_cash = user_cash - cost_stock
                #updates table to update user cash
                db.execute("UPDATE users SET cash = ? WHERE username = ?", user_cash, session["username"])
                session["page_status"] = "bought"
                return redirect("/")
            else:
                #user does not have enough funds
                return apology("user does not have sufficient funds", 400)
        else:
            #lookup returned NULL
            return apology("invalid stock symbol", 400)

    #get request
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE username = ?", session["username"])
    print(transactions)
    return render_template("history.html", transactions=transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = request.form.get("username")
        session["page_status"] = "none"

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

@app.route("/changepassword", methods=["GET", "POST"])
def changepw():
    """ChangePW"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide old password", 400)
        # Ensure new password was submitted
        elif not request.form.get("newpassword"):
            return apology("must provide new password", 400)
        # Ensure password was confirmed
        elif not request.form.get("confirmation"):
            return apology("must confirm new password", 400)

        #ensure passwords match
        elif request.form.get("newpassword") != request.form.get("confirmation"):
            return apology("new passwords must match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)
        #changes password in database
        db.execute("UPDATE users SET hash = ? WHERE username = ?;", generate_password_hash(request.form.get("newpassword")),request.form.get("username"))

        #returns to login
        return render_template("login.html")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("changepw.html")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        #checks if symbol was submitted
        if request.form.get("symbol"):
            #looks up stock and returns dict with keys name, price, symbol
            stock_dict = lookup(request.form.get("symbol"))
            #checks if stock was found during lookup
            if (stock_dict):
                return render_template("quoted.html", stock=stock_dict)
            else:
                return apology("invalid stock symbol", 400)
        #else symbol was blank
        else:
            return apology("must provide stock symbol", 400)
    #under get request, asks for stock symbol in quote.html
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        #checks if username was submitted and is unique
        if request.form.get("username"):
            # Query database for username
            rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
            #checks if database already has that username
            if len(rows) != 0:
                return apology("username already exists", 400)

        # if username not submitted
        elif not request.form.get("username"):
            return apology("must provide username", 400)

        #new if statements
        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 400)

        #adds user to database - only gets executed if all above tests are passed
        else:
            db.execute("INSERT INTO users(username, hash) VALUES(?,?);", request.form.get("username"), generate_password_hash(request.form.get("password")))
            return render_template("login.html")

        # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    #TODO - NEEEDS TO CHECK IF NUMBER OF STOCKS BEING SOLD IS <= NUMBER OF STOCKS I HAVE
    #90% same as for buying
    if request.method == "POST":
        #checks if symbol was submitted
        if not request.form.get("symbol"):
            #if symbol was blank
            return apology("must provide stock symbol", 400)

        #checks fractional shares
        try:
            int(request.form.get("shares"))
        except ValueError:
            return apology("number of shares must be an integer", 400)

        #checks if number of shares is not a positive number (e.g., blank)
        if not int(request.form.get("shares")) > 0:
            return apology("must provide a positive number of shares", 400)

        #checks if user has enough shares to sell
        num_shares_held = db.execute("SELECT SUM(shares) FROM transactions WHERE username = ? AND symbol = ?", session["username"], request.form.get("symbol"))[0]["SUM(shares)"]

        #checks if user has any shares of the stock
        if not int(num_shares_held):
            return apology("user does not have any shares of that stock", 400)

        #checks if user has enough shares
        if int(request.form.get("shares")) > int(num_shares_held):
            return apology("user does not have enough shares to sell that amount", 400)

        #negative shares for selling
        shares = -1 * int(request.form.get("shares"))
        #looks up stock and returns dict with keys name, price, symbol
        stock_dict = lookup(request.form.get("symbol"))
        #adds sale to transactions
        db.execute("INSERT INTO transactions(username, symbol, name, shares, price, time) VALUES(?,?,?,?,?,?);", session["username"], stock_dict["symbol"], stock_dict["name"], shares, stock_dict["price"], str(datetime.utcnow())[0:-7])
        #checks user cash
        user_cash = db.execute("SELECT cash FROM users WHERE username = ?", session["username"])[0]["cash"]
        #sale value of stock
        value_stock = float(request.form.get("shares")) * float(stock_dict["price"])
        #adds value of stock to user cash
        user_cash = user_cash + value_stock
        #updates table to update user cash
        db.execute("UPDATE users SET cash = ? WHERE username = ?", user_cash, session["username"])
        session["page_status"] = "sold"
        return redirect("/")

    #get request
    else:
        #symbols = db.execute("SELECT symbol FROM transactions WHERE username = ?", session["username"])
        symbols = db.execute("SELECT symbol FROM (SELECT symbol, SUM(shares) FROM transactions WHERE username = ? GROUP BY symbol HAVING SUM(shares) > 0)", session["username"])
        print(symbols)
        return render_template("sell.html", symbols=symbols)

