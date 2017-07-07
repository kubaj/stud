import datetime
from decimal import Decimal
from re import sub

def get_tommorow_date():
    tommorow = datetime.date.today() + datetime.timedelta(days=1) 
    return tommorow.strftime('%Y-%m-%d')

def get_value_from_price(price):
    return int(Decimal(sub(r'[^\d\-.]', '', price)))