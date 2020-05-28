import java.sql.*;
import java.time.LocalDate;

public class DbConnectionImpl implements DbConnection{
    static String connectionStr =
            "jdbc:sqlserver://sepfour.database.windows.net:1433;database=SEP4;user=superuser@sepfour;password=28052020Ul;encrypt=true;trustServerCertificate=false;hostNameInCertificate=*.database.windows.net;loginTimeout=30;";
    public DbConnectionImpl(){

    }

    @Override
    public void insert(float co2, float humidity, float temperature, int roomId, float noise) {
        LocalDate date = LocalDate.now();
        String insertSql = "INSERT INTO [dbo].[Metrics] (Humidity, Temperature, Noise, CO2, LastUpdated, R_ID) " +
                "VALUES (" + humidity +", " + temperature + ", "+ noise+", "+ co2+", "+date+" , "+roomId+")";
        ResultSet resultSet = null;
        try (Connection connection = DriverManager.getConnection(connectionStr);
             PreparedStatement prepsInsertProduct = connection.prepareStatement(insertSql, Statement.RETURN_GENERATED_KEYS);)
        {

            prepsInsertProduct.execute();
            // Retrieve the generated key from the insert.
            resultSet = prepsInsertProduct.getGeneratedKeys();

            // Print the ID of the inserted row.
            if(resultSet  !=   null)
            {
                System.out.println("Hjelp");
            }
        }
        // Handle any errors that may have occurred.
        catch (Exception e) {
            e.printStackTrace();
        }
    }


}