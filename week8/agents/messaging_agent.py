import os
from agents.deals import Opportunity
from agents.agent import Agent
import requests
from openai import OpenAI

# Disable LiteLLM's automatic model routing by setting environment variables
# This prevents LiteLLM from intercepting OpenAI calls
os.environ.setdefault("LITELLM_TURN_OFF_MESSAGE_LOGGING", "True")
os.environ.setdefault("LITELLM_LOG", "ERROR")

pushover_url = "https://api.pushover.net/1/messages.json"


class MessagingAgent(Agent):
    name = "Messaging Agent"
    color = Agent.WHITE
    #MODEL = "claude-sonnet-4-5"
    MODEL = "gpt-5.1"

    def __init__(self):
        """
        Set up this object to either do push notifications via Pushover,
        or SMS via Twilio,
        whichever is specified in the constants
        """
        self.log("Messaging Agent is initializing")
        self.pushover_user = os.getenv("PUSHOVER_USER", "your-pushover-user-if-not-using-env")
        self.pushover_token = os.getenv("PUSHOVER_TOKEN", "your-pushover-user-if-not-using-env")
        # Explicitly use OpenAI's official endpoint to bypass LiteLLM interception
        api_key = os.getenv("OPENAI_API_KEY")
        # Create OpenAI client with explicit base_url to ensure direct connection
        # Note: If LiteLLM is monkey-patching, you may need to restart the Python kernel
        self.openai = OpenAI(
            api_key=api_key,
            base_url="https://api.openai.com/v1"  # Explicit OpenAI endpoint to bypass LiteLLM
        )
        self.log("Messaging Agent has initialized Pushover")

    def push(self, text):
        """
        Send a Push Notification using the Pushover API
        """
        self.log("Messaging Agent is sending a push notification")
        payload = {
            "user": self.pushover_user,
            "token": self.pushover_token,
            "message": text,
            "sound": "cashregister",
        }
        requests.post(pushover_url, data=payload)

    def alert(self, opportunity: Opportunity):
        """
        Make an alert about the specified Opportunity
        """
        text = f"Deal Alert! Price=${opportunity.deal.price:.2f}, "
        text += f"Estimate=${opportunity.estimate:.2f}, "
        text += f"Discount=${opportunity.discount:.2f} :"
        text += opportunity.deal.product_description[:10] + "... "
        text += opportunity.deal.url
        self.push(text)
        self.log("Messaging Agent has completed")

    def craft_message(
        self, description: str, deal_price: float, estimated_true_value: float
    ) -> str:
        user_prompt = "Please summarize this great deal in 2-3 sentences to be sent as an exciting push notification alerting the user about this deal.\n"
        user_prompt += f"Item Description: {description}\nOffered Price: {deal_price}\nEstimated true value: {estimated_true_value}"
        user_prompt += "\n\nRespond only with the 2-3 sentence message which will be used to alert & excite the user about this deal"
        response = self.openai.chat.completions.create(
            model=self.MODEL,
            messages=[
                {"role": "user", "content": user_prompt},
            ],
        )
        return response.choices[0].message.content

    def notify(self, description: str, deal_price: float, estimated_true_value: float, url: str):
        """
        Make an alert about the specified details
        """
        self.log("Messaging Agent is using OpenAI to craft the message")
        text = self.craft_message(description, deal_price, estimated_true_value)
        self.push(text[:200] + "... " + url)
        self.log("Messaging Agent has completed")
