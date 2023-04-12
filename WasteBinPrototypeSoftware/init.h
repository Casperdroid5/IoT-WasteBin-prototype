class main
{
private:
public:
    /// \brief initalize main program
    void init();
    /// \brief run main program
    void program();
    /// \brief connect to wifi network using wifi credentials
    void connecttowifi();
};

class sensor
{
private:
    float Temperature = 0;
    float Humidity = 0;
    bool  lidposition = true;

public:
    /// \brief get degrees celsius from temperature
    void updateTemperature();
    /// \brief get %RH form humidity sensor
    void updateHumidity();
    /// \brief get ppm from gas sensor
};