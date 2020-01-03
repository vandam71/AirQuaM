import datetime as dt


class Timer:
    """General Use Timer"""

    def __init__(self, context):
        """Start the timer with no value
        :param context: Extra information about the timer usage
        """
        self.start_dt = None
        self.context = context

    def start(self):
        """Start the Timer"""
        self.start_dt = dt.datetime.now()
        return self

    def stop(self):
        """Stop and Print the Timer difference"""
        end_dt = dt.datetime.now()
        print(f'[{self.context}] Time taken: %s' % (end_dt - self.start_dt))
