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
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        # Validate
        if not request.form.get("name"):
            return render_template("error.html", message="Name is missing")
        if not request.form.get("birthday"):
            return render_template("error.html", message="Date is missing")
        else:
            month = int(request.form.get("birthday")[5:7])
            day = int(request.form.get("birthday")[8:])
            if month < 1 or month > 12 or day < 1 or day > 31:
                return render_template("error.html", message="Invalid date")
            db.execute(
                "INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)",
                request.form.get("name"),
                month,
                day,
            )
        return redirect("/")

    else:
        rows = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", rows=rows)
