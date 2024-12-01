FROM maven:3.8.6-openjdk-11 AS build
COPY . /app
WORKDIR /app
RUN mvn package -DskipTests

FROM openjdk:11-jre-slim
COPY --from=build /app/wait-for-it.sh /wait-for-it.sh
RUN chmod +x /wait-for-it.sh
COPY --from=build /app/target/*.jar app.jar
ENTRYPOINT ["./wait-for-it.sh", "${POSTGRES_HOST}:${POSTGRES_PORT}", "--", "java", "-jar", "yourapp.jar"]
