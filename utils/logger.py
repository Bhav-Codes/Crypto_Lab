"""
Logger utility for recording user actions
"""

import os
from datetime import datetime


class Logger:
    """Handles logging of user actions to a log file"""
    
    def __init__(self, log_dir="outputs"):
        """Initialize logger with log directory"""
        self.log_dir = os.path.join(os.path.dirname(os.path.dirname(__file__)), log_dir)
        self.log_file = os.path.join(self.log_dir, "activity.log")
        
        # Create log directory if it doesn't exist
        os.makedirs(self.log_dir, exist_ok=True)
        
        # Create log file if it doesn't exist
        if not os.path.exists(self.log_file):
            with open(self.log_file, 'w') as f:
                f.write("CryptoLabX Activity Log\n")
                f.write("=" * 70 + "\n")
    
    def log(self, action):
        """
        Log an action with timestamp
        
        Args:
            action (str): The action performed by user
        """
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log_entry = f"[{timestamp}] Action: {action}\n"
        
        try:
            with open(self.log_file, 'a') as f:
                f.write(log_entry)
        except Exception as e:
            print(f"Warning: Could not write to log file: {e}")
    
    def get_log_path(self):
        """Return the path to the log file"""
        return self.log_file
