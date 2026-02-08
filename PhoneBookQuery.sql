
IF NOT EXISTS (SELECT name FROM sys.databases WHERE name = 'PhoneBook')
BEGIN
    CREATE DATABASE PhoneBook;
END;
GO



USE PhoneBook;
GO

IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'CITIES')
BEGIN
    CREATE TABLE [CITIES] (
        [ID] INT IDENTITY(1,1),
        [UPDATE_COUNTER] INT DEFAULT 0,
        [NAME] NVARCHAR(16) NOT NULL,
        [REGION] NVARCHAR(16) NOT NULL,
        CONSTRAINT [PK_CITIES_ID] PRIMARY KEY ([ID]),
		CONSTRAINT [UX_CITIES_NAME_REGION] UNIQUE ([NAME], [REGION])
    );
	CREATE INDEX IX_CITIES_REGION ON CITIES ([REGION])
END;
GO
DROP TABLE CITIES;
GO


IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'PHONE_TYPES')
BEGIN
    CREATE TABLE [PHONE_TYPES] (
        [ID] INT IDENTITY(1,1),
        [UPDATE_COUNTER] INT DEFAULT 0,
        [TYPE] NVARCHAR(16),
        CONSTRAINT [PK_PHONE_TYPES_ID] PRIMARY KEY ([ID]),
    );
	CREATE UNIQUE INDEX [UX_PHONE_TYPES_TYPE] ON PHONE_TYPES([TYPE])
END;
GO

IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'PERSONS')
BEGIN
    CREATE TABLE [PERSONS] (
        [ID] INT IDENTITY(1,1),
        [UPDATE_COUNTER] INT DEFAULT 0,
        [FIRST_NAME] NVARCHAR(32) NOT NULL,
        [MIDDLE_NAME] NVARCHAR(32) NOT NULL,
        [LAST_NAME] NVARCHAR(32) NOT NULL,
        [CITIZEN_ID] NVARCHAR(16) NOT NULL,
        [CITY_ID] INT NOT NULL,
        [ADDRESS] NVARCHAR(64) NOT NULL,

        CONSTRAINT [PK_PERSONS_ID] PRIMARY KEY ([ID]),
        CONSTRAINT [FK_PERSONS_CITY_ID] FOREIGN KEY ([CITY_ID]) REFERENCES [CITIES] ([ID])
		)
		
		CREATE UNIQUE INDEX [UX_PERSONS_CITIZEN_ID] ON PERSONS (CITIZEN_ID)
		CREATE INDEX [IX_PERSONS_CITY_ID] ON PERSONS(CITY_ID)
    
END;
GO

IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'PHONE_NUMBERS')
BEGIN
    CREATE TABLE [PHONE_NUMBERS] (
		
        [ID] INT IDENTITY(1,1),
		[UPDATE_COUNTER] INT DEFAULT 0,
        [PERSON_ID] INT NOT NULL,
        [PHONE_TYPE_ID] INT NOT NULL,
        [PHONE_NUMBER] NVARCHAR(16) UNIQUE NOT NULL,
        FOREIGN KEY ([PERSON_ID]) REFERENCES [PERSONS] ([ID]),
        FOREIGN KEY ([PHONE_TYPE_ID]) REFERENCES [PHONE_TYPES] ([ID]),
		CONSTRAINT [UX_PHONE_NUMBERS_PHONE_NUMBER] UNIQUE ([PHONE_NUMBER]),
		CONSTRAINT [PK_PHONE_NUMBERS_ID] PRIMARY KEY ([ID])

    );
	CREATE UNIQUE INDEX [UX_PHONE_NUMBERS_PERSON_ID_PHONE_TYPE_ID] ON [PHONE_NUMBERS] ([PERSON_ID], [PHONE_TYPE_ID])

END;
GO

SELECT * FROM PERSONS;
GO

SELECT * FROM CITIES;
GO

	DELETE FROM [CITIES] WHERE ID = 8;
	GO;
SELECT * FROM PHONE_NUMBERS;
GO

DELETE FROM PERSONS WHERE ID = 1004;
GO
SELECT * FROM CITIES WHERE ID = 1;
GO

INSERT	[CITIES] 
	([UPDATE_COUNTER],[NAME], REGION) 
VALUES 
(0,'София','Левскис'),
(0,'Пловдив', 'Южен'),
(0,'Варна', 'Североизточен'),
(0,'Бургас', 'Югоизточен'),
(0,'Русе', 'Северен'),
(0,'Стара Загора', 'Южен'),
(0,'Плевен', 'Северен');
GO

INSERT INTO PHONE_TYPES ([UPDATE_COUNTER],[TYPE]) VALUES 
(0,'Мобилен'),
(0,'Домашен'),
(0,'Служебен'),
(0,'Факс'),
(0,'Пейджър');


INSERT INTO [PERSONS] 
    ([UPDATE_COUNTER],[FIRST_NAME], [MIDDLE_NAME], [LAST_NAME], [CITIZEN_ID], [CITY_ID], [ADDRESS]) 
VALUES 
    (0,'Иван', 'Петров', 'Иванов', '9001010001', 1, 'ул. Витошка 15'),
    (0,'Мария', 'Георгиева', 'Димитрова', '9202020002', 1, 'бул. България 8'),
    (0,'Георги', 'Иванов', 'Петров', '9103030003', 2, 'ул. Черно море 12'),
    (0,'Елена', 'Симеонова', 'Костова', '9304040004', 6, 'бул. Лазур 3'),
    (0,'Димитър', 'Стефанов', 'Петров', '8805050005', 4, 'бул. Цар Освободител 10'),
    (0,'Стефан', 'Иванов', 'Георгиев', '9506060006', 5, 'ул. Капитан Райчо 25'),
    (0,'Анна', 'Петрова', 'Симеонова', '9707070007', 7, 'ул. Шипка 5'),
    (0,'Николай', 'Георгиев', 'Иванов', '8508080008', 2, 'бул. Демокрация 50'),
    (0,'Петър', 'Димитров', 'Костов', '9909090009', 2, 'ул. Александровска 120'),
    (0,'Ивелина', 'Симеонова', 'Димова', '9410100010', 5, 'бул. Никола Петков 30'),
    (0,'Георги', 'Петров', 'Ангелов', '8011110011', 3, 'пл. Възраждане 1'),
    (0,'Мария', 'Иванова', 'Тодорова', '9612120012', 2, 'ул. Граф Игнатиев 18');

	INSERT INTO [PHONE_NUMBERS] 
    ([PERSON_ID], [PHONE_TYPE_ID], [PHONE_NUMBER]) 
VALUES 
    (2, 1, '0888123456'),  -- Иван - мобилен
    (2, 2, '029876543'),   -- Иван - домашен
    (3, 1, '0899111222'),  -- Мария - мобилен
    (4, 3, '052123456'),   -- Георги - служебен
    (5, 1, '0877123123'),  -- Елена - мобилен
    (6, 2, '02111222'),    -- Димитър - домашен
    (6, 3, '02333444'),    -- Димитър - служебен
    (7, 1, '0877777777'),  -- Стефан - мобилен
    (8, 2, '052888999'),   -- Анна - домашен
    (9, 3, '056112233'),   -- Николай - служебен
    (10, 1, '0899555666'),  -- Петър - мобилен
    (10, 2, '082123123'),   -- Петър - домашен
    (11, 1, '0888999000'), -- Ивелина - мобилен
    (12, 2, '064555444'),  -- Георги - домашен
    (13, 1, '0895112233'), -- Мария - мобилен
    (13, 3, '02444555');   -- Мария - служебен


	INSERT INTO [PHONE_NUMBERS] 
    ([PERSON_ID], [PHONE_TYPE_ID], [PHONE_NUMBER]) 
VALUES 
(NULL,NULL,'0882993000');

SELECT * FROM CITIES 
-- Подобрения
INSERT INTO PERSONS (FIRST_NAME, LAST_NAME, CITY_ID)
VALUES ('Иван', 'Василев', (SELECT ID FROM CITIES WHERE NAME = 'Пловдив'));

INSERT INTO PHONE_NUMBERS (PERSON_ID, PHONE_TYPE_ID, PHONE_NUMBER)
VALUES (13, (SELECT ID FROM PHONE_TYPES WHERE TYPE = 'Мобилен'), '0887112233');

-- Updates
UPDATE [PERSONS]
SET [ADDRESS] = 'ул. Витошка 20'
WHERE [CITIZEN_ID] = '9001010001';

--DELETE
DELETE FROM [PHONE_NUMBERS]
WHERE [PERSON_ID] = 1 AND [PHONE_TYPE_ID] = 1;

DELETE FROM [PERSONS]
WHERE [CITIZEN_ID] = '9001010001';

--Извличане на всички телефонни номера за даден клиент:
SELECT [PHONE_TYPE_ID], [PHONE_NUMBER]
FROM [PHONE_NUMBERS]
WHERE [PERSON_ID] = 1;

--Промени
ALTER TABLE CITIES
ADD CONSTRAINT UQ_CITIES_NAME_REGION UNIQUE (NAME, REGION);

ALTER TABLE PHONE_NUMBERS
ADD CONSTRAINT UQ_PERSON_PHONE_TYPE UNIQUE (PERSON_ID, PHONE_TYPE_ID);


-- Транзакции 

BEGIN TRANSACTION;

DELETE FROM [PHONE_NUMBERS]
WHERE [PERSON_ID] = 1;

DELETE FROM [PERSONS]
WHERE [ID] = 1;

COMMIT TRANSACTION;


BEGIN TRY
    BEGIN TRANSACTION;

    DELETE FROM [PHONE_NUMBERS]
    WHERE [PERSON_ID] = 1;

    DELETE FROM [PERSONS]
    WHERE [ID] = 1;

    COMMIT TRANSACTION;

END TRY
BEGIN CATCH
    ROLLBACK TRANSACTION;

END CATCH;

-- Ако възникне грешка, възстановяване на предишното състояние
-- ROLLBACK TRANSACTION;

-- Queries
--Проверка и извличане на първото и последното име на хората и типа на телефонния номер:
SELECT
        [P].[FIRST_NAME],
        [P].[LAST_NAME],
        [PT].[TYPE]
    FROM
        [PERSONS] [P]
    INNER JOIN
        [PHONE_NUMBERS] [PN] ON [P].[ID] = [PN].[PERSON_ID]
    INNER JOIN
        [PHONE_TYPES] [PT] ON [PN].[PHONE_TYPE_ID] = [PT].[ID];
GO
-- Проверка и извличане на името на града и имената на хората, които живеят в този град:
SELECT
        [C].[NAME],
        [P].[FIRST_NAME],
        [P].[LAST_NAME]
    FROM
        [PERSONS] [P]
    RIGHT JOIN
        [CITIES] [C] ON [P].[CITY_ID] = [C].[ID];
GO

-- Проверка и извличане на типа на телефона, името и фамилията на хората, които имат телефонен номер:
SELECT
        [PT].[TYPE],
        [P].[FIRST_NAME],
        [P].[LAST_NAME]
    FROM
        [PERSONS] [P]
    RIGHT JOIN
        [PHONE_NUMBERS] [PN] ON [P].[ID] = [PN].[PERSON_ID]
    RIGHT JOIN
        [PHONE_TYPES] [PT] ON [PN].[PHONE_TYPE_ID] = [PT].[ID];
GO

--	Проверка и извличане на всички хора, техните ЕГН-та, градове, телефонни номера и типове телефони:
SELECT 
        [P].[FIRST_NAME], [P].[LAST_NAME], [P].[CITIZEN_ID], [C].[NAME], [PN].[PHONE_NUMBER], [PT].[TYPE]
    FROM 
        [PERSONS] [P]
    LEFT JOIN 
        [CITIES] [C] ON [P].[CITY_ID] = [C].[ID]
    LEFT JOIN 
        [PHONE_NUMBERS] [PN] ON [P].[ID] = [PN].[PERSON_ID]
    LEFT JOIN 
        [PHONE_TYPES] [PT] ON [PN].[PHONE_TYPE_ID] = [PT].[ID];
GO

-- Проверка и извличане на всички хора от град "Варна" с телефонни номера и типове телефони:
SELECT
        [P].[FIRST_NAME], [P].[LAST_NAME], [P].[CITIZEN_ID], [C].[NAME], [PT].[TYPE]
    FROM
        [PERSONS] [P]
    LEFT JOIN
        [CITIES] [C] ON [P].[CITY_ID] = [C].[ID]
    LEFT JOIN
        [PHONE_NUMBERS] [PN] ON [P].[ID] = [PN].[PERSON_ID]
    LEFT JOIN
        [PHONE_TYPES] [PT] ON [PN].[PHONE_TYPE_ID] = [PT].[ID]
    WHERE
        [C].[NAME] = 'Варна';
GO
-- Домашно
-- 1.1 Изведи информация за person-и, които нямат телефонни номера
SELECT 
        [P].[FIRST_NAME], 
        [P].[LAST_NAME], 
        [P].[CITIZEN_ID], 
        [C].[NAME], 
        [P].[ADDRESS]
    FROM 
        [PERSONS] [P]
    LEFT JOIN 
        [PHONE_NUMBERS] [PN] ON [P].[ID] = [PN].[PERSON_ID]
    LEFT JOIN 
        [CITIES] [C] ON [P].[CITY_ID] = [C].[ID]
    WHERE 
        [PN].[PERSON_ID] IS NULL;
GO

-- 1.2. Изведи информация за телефонни номера, които нямат свързан person:
SELECT 
        [PN].[PHONE_NUMBER], 
        [PT].[TYPE]
    FROM 
        [PHONE_NUMBERS] [PN]
    LEFT JOIN 
        [PERSONS] [P] ON [PN].[PERSON_ID] = [P].[ID]
    LEFT JOIN 
        [PHONE_TYPES] [PT] ON [PN].[PHONE_TYPE_ID] = [PT].[ID]
    WHERE 
        [P].[ID] IS NULL;
GO

-- 1.3 Изведи имената на всички градове, в които има хора с домашни телефони:
SELECT DISTINCT
        [C].[NAME]
    FROM 
        [PERSONS] [P]
    INNER JOIN 
        [PHONE_NUMBERS] [PN] ON [P].[ID] = [PN].[PERSON_ID]
    INNER JOIN 
        [PHONE_TYPES] [PT] ON [PN].[PHONE_TYPE_ID] = [PT].[ID]
    INNER JOIN 
        [CITIES] [C] ON [P].[CITY_ID] = [C].[ID]
    WHERE 
        [PT].[TYPE] = 'Домашен';
GO

-- Пример с GroupBy
-- Групиране по тип телефон и извеждане на броя на хората с даден тип телефон:
SELECT 
        [PT].[TYPE], 
        COUNT([P].[ID]) AS [NUMBER_OF_PEOPLE]
    FROM 
        [PERSONS] [P]
    INNER JOIN 
        [PHONE_NUMBERS] [PN] ON [P].[ID] = [PN].[PERSON_ID]
    INNER JOIN 
        [PHONE_TYPES] [PT] ON [PN].[PHONE_TYPE_ID] = [PT].[ID]
    GROUP BY 
        [PT].[TYPE];
GO

-- Групиране по град и извеждане на броя на хората в този град:
SELECT 
        [C].[NAME], 
        COUNT([P].[ID]) AS [NUMBER_OF_PEOPLE]
    FROM 
        [PERSONS] [P]
    INNER JOIN 
        [CITIES] [C] ON [P].[CITY_ID] = [C].[ID]
    GROUP BY 
        [C].[NAME];
GO


-- Views

IF NOT EXISTS (SELECT * FROM sys.views WHERE name = 'VI_PersonsWithCity')
BEGIN
    CREATE VIEW VI_PersonsWithCity AS
    SELECT P.FIRST_NAME, P.LAST_NAME, C.NAME AS CITY_NAME
    FROM PERSONS P
    JOIN CITIES C ON P.CITY_ID = C.ID;
END;
GO



IF NOT EXISTS (SELECT * FROM sys.views WHERE name = 'VI_PhoneNumbersWithPersons')
BEGIN
    CREATE VIEW VI_PhoneNumbersWithPersons AS
    SELECT PN.PHONE_NUMBER, PT.TYPE AS PHONE_TYPE, P.FIRST_NAME, P.LAST_NAME, C.NAME AS CITY_NAME
    FROM PHONE_NUMBERS PN
    JOIN PERSONS P ON PN.PERSON_ID = P.ID
    JOIN PHONE_TYPES PT ON PN.PHONE_TYPE_ID = PT.ID
    LEFT JOIN CITIES C ON P.CITY_ID = C.ID;
END;
GO


IF NOT EXISTS (SELECT * FROM sys.views WHERE name = 'VI_PersonsWithoutPhones')
BEGIN
   CREATE VIEW VI_PersonsWithoutPhones AS
    SELECT P.ID AS PERSON_ID, P.FIRST_NAME, P.LAST_NAME, C.NAME AS CITY_NAME
    FROM PERSONS P
    LEFT JOIN PHONE_NUMBERS PN ON P.ID = PN.PERSON_ID
    LEFT JOIN CITIES C ON P.CITY_ID = C.ID
    WHERE PN.ID IS NULL;
END;
GO


-- Таблица CITIES
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Съдържа информация за градовете, включително име и регион.', 
    @level0type = N'SCHEMA', @level0name = 'dbo',
    @level1type = N'TABLE',  @level1name = 'CITIES';
	-- CITIES.ID
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Уникален идентификатор на града.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'CITIES',
    @level2type = N'COLUMN', @level2name = 'ID';

-- CITIES.NAME
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Име на града.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'CITIES',
    @level2type = N'COLUMN', @level2name = 'NAME';

-- CITIES.REGION
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Регион, в който се намира градът.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'CITIES',
    @level2type = N'COLUMN', @level2name = 'REGION';


-- Таблица PHONE_TYPES
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Съдържа типовете телефони – например мобилен, домашен, служебен и др.', 
    @level0type = N'SCHEMA', @level0name = 'dbo',
    @level1type = N'TABLE',  @level1name = 'PHONE_TYPES';
	-- PHONE_TYPES.ID
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Уникален идентификатор на типа телефон.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PHONE_TYPES',
    @level2type = N'COLUMN', @level2name = 'ID';

-- PHONE_TYPES.NAME
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Име/описание на типа телефон (напр. мобилен, домашен).', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PHONE_TYPES',
    @level2type = N'COLUMN', @level2name = 'NAME';


-- Таблица PERSONS
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Съдържа информация за хората – включително три имена, ЕГН, адрес и град.', 
    @level0type = N'SCHEMA', @level0name = 'dbo',
    @level1type = N'TABLE',  @level1name = 'PERSONS';
	-- PERSONS.ID
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Уникален идентификатор на човек.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PERSONS',
    @level2type = N'COLUMN', @level2name = 'ID';

-- PERSONS.FULL_NAME
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Пълното име на човека (три имена).', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PERSONS',
    @level2type = N'COLUMN', @level2name = 'FULL_NAME';

-- PERSONS.EGN
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Единен граждански номер (ЕГН) на човека.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PERSONS',
    @level2type = N'COLUMN', @level2name = 'EGN';

-- PERSONS.ADDRESS
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Адрес на човека.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PERSONS',
    @level2type = N'COLUMN', @level2name = 'ADDRESS';



-- PERSONS.CITY_ID
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Идентификатор на града от таблицата CITIES.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PERSONS',
    @level2type = N'COLUMN', @level2name = 'CITY_ID';


-- Таблица PHONE_NUMBERS
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Свързва хора с телефонни номера и типове телефони.', 
    @level0type = N'SCHEMA', @level0name = 'dbo',
    @level1type = N'TABLE',  @level1name = 'PHONE_NUMBERS';
	-- PHONE_NUMBERS.ID
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Уникален идентификатор на телефонен запис.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PHONE_NUMBERS',
    @level2type = N'COLUMN', @level2name = 'ID';

-- PHONE_NUMBERS.PERSON_ID
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Идентификатор на човека от таблицата PERSONS.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PHONE_NUMBERS',
    @level2type = N'COLUMN', @level2name = 'PERSON_ID';

-- PHONE_NUMBERS.PHONE_TYPE_ID
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Идентификатор на типа телефон от таблицата PHONE_TYPES.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PHONE_NUMBERS',
    @level2type = N'COLUMN', @level2name = 'PHONE_TYPE_ID';

-- PHONE_NUMBERS.PHONE_NUMBER
EXEC sp_addextendedproperty 
    @name = N'MS_Description', 
    @value = N'Телефонният номер.', 
    @level0type = N'SCHEMA', @level0name = 'dbo', 
    @level1type = N'TABLE',  @level1name = 'PHONE_NUMBERS',
    @level2type = N'COLUMN', @level2name = 'PHONE_NUMBER';