# SkyInSight: Your Personal Weather Dashboard

SkyInSight offers a concise and user-friendly way to get weather forecasts right at your terminal. Leveraging the FTXUI library, it presents weather data in an appealing, text-based interface. Whether you're planning your day or just curious about the weather in various cities, SkyInSight provides the information you need at a glance.

## 🌤️ About 🌤️ 

SkyInSight is a C++ application that utilizes the FTXUI library to fetch and display weather information in your terminal. It's designed to be simple, efficient, and easy to use, making it a go-to choice for developers, system administrators, and anyone who prefers working within a terminal environment. With SkyInSight, you get access to current weather conditions, temperatures, and forecasts with minimal hassle.

## 🛠️ Installation 🛠️

To install SkyInSight, follow these steps:

1. **Clone the Repository**

   Begin by cloning the SkyInSight repository to your local machine.

   ```bash
   git clone https://github.com/YourUsername/SkyInSight.git
   ```

2. **Install Dependencies**

   SkyInSight relies on the FTXUI library for its user interface. Install FTXUI with the following commands:

   ```bash
   cd SkyInSight
   sudo make install
   ```

3. **Compile SkyInSight**

   With the dependencies installed, compile SkyInSight using the provided Makefile:

   ```bash
   make all
   ```

## 🚀 Quick How-To Use 🚀

To get started with SkyInSight, use the following commands in your terminal:

- **Help**

  ```bash
  ./SkyInsight -h
  ```

  This command displays the help menu, listing all available commands and usage instructions.

- **Version**

  ```bash
  ./SkyInsight -v
  ```

  Check the current version of SkyInSight and some additional info.

- **Set API Key**

  ```bash
  ./SkyInsight -k YOUR_API_KEY
  ```

  Before fetching weather data, you need to set your API key from the weather service provider. [API LINK](https://www.weatherapi.com/docs/)

- **Specify City**

  ```bash
  ./SkyInsight -c "New York"
  ```

  Replace `"New York"` with the name of your city. This command fetches and displays the weather for the specified city.

- **Use Filters**

  ```bash
  ./SkyInsight -f tw
  ```

  The `-f` option allows you to specify what weather information you want to see. For example, `t` for temperature, `w` for weather conditions.

SkyInSight is designed to be intuitive for anyone who frequently works in the terminal. With just a few commands, you can access comprehensive weather information without ever leaving your console.