from selenium import webdriver
from dashboard_page import DashboardPage
from base_page import BasePage

class LoginPage(BasePage):
    def __init__(self, driver: webdriver):
        self.driver = driver
        
    def login(self, username, password):
        self.fill_username(username)
        self.fill_password(password)
        self.push_login()
        return DashboardPage(self.driver)
    
    def fill_username(self, username):
        self.driver.find_element_by_id('input-username').clear()
        self.driver.find_element_by_id('input-username').send_keys(username)

    def fill_password(self, password):
        self.driver.find_element_by_id('input-password').clear()
        self.driver.find_element_by_id('input-password').send_keys(password)

    def push_login(self):
        self.driver.find_element_by_xpath("//button[@type='submit']").click()
