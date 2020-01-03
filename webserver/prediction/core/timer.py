import datetime as dt


class Timer:
    """General Use Timer"""

    def __init__(self):
        """Start the timer with no value"""
        self.start_dt = None

    def start(self):
        """Start the Timer"""
        self.start_dt = dt.datetime.now()
        return self

    def stop(self):
        """Stop and Print the Timer difference"""
        end_dt = dt.datetime.now()
        print('Time taken: %s' % (end_dt - self.start_dt))
