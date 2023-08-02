import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    birthdays1 = db.execute("SELECT name, month, day FROM birthdays;")
    if request.method == "POST":
        # TODO: Add the user's entry into the database
        name1 = request.form.get("name")
        month1 = request.form.get("month")
        day1 = request.form.get("day")
        if (name1 and month1 and day1):
            db.execute("INSERT INTO birthdays(name,month,day) VALUES(?,?,?);", name1, month1, day1)
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        return render_template("index.html", birthdays=birthdays1)


