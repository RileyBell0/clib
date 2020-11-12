#include "../configLoader.h"

/*
 * returns the position in the string the function exited at
 * 
 * complexString is set to TRUE iff the returned string was surrounded by quotes 
*/
int extract_field(string_t str, unsigned int *pos, string_t *dest, int *complexField)
{
    dest->len = 0;
    *complexField = FALSE;
    unsigned int i;
    if (str.len <= *pos)
    {
        return FALSE;
    }

    /*
     * Ignore leading whitespaces and tabs
    */
    for (i = *pos; i < str.len; i++)
    {
        if (str.str[i] != CONFIG_IGNORE_CHAR && str.str[i] != '\t')
        {
            // Done with leading whitespaces
            break;
        }
    }
    *pos = i;

    /*
     * If we've reached an = as its own field
    */
    if (str.str[*pos] == CONFIG_FIELD_DECLARATION_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If we've reached a # as its own field
    */
    if (str.str[*pos] == CONFIG_COMMENT_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If we've reached an array declaration square brace
    */
    if (str.str[*pos] == CONFIG_ARRAY_START_CHAR || str.str[*pos] == CONFIG_ARRAY_END_CHAR)
    {
        string_write_char(dest, str.str[*pos]);
        string_null_terminate(dest);
        *pos += 1;
        return TRUE;
    }

    /*
     * If the current field is within quotes
    */
    int inQuotes = FALSE;
    if (str.str[*pos] == CONFIG_STRING_ENCLOSE_CHAR)
    {
        *complexField = TRUE;
        *pos += 1;
        inQuotes = TRUE;
    }

    if (inQuotes)
    {
        /*
         * Read until the string is escaped
        */
        for (i = *pos; i < str.len; i++)
        {
            char c = str.str[i];

            if (c == CONFIG_STRING_ENCLOSE_CHAR)
            {
                // Move to the next character then exit the loop
                i++;
                break;
            }
            if (c == CONFIG_ESCAPE_CHAR && str.str[i + 1] == CONFIG_STRING_ENCLOSE_CHAR)
            {
                // reached a quote intended to be within a string
                string_write_char(dest, str.str[i + 1]);
                i++;
            }
            else
            {
                string_write_char(dest, c);
            }
        }
    }
    else
    {
        /*
         * Read until a whitespace or special char related to
         * the config file is read
        */
        for (i = *pos; i < str.len; i++)
        {
            char c = str.str[i];

            if (c == CONFIG_IGNORE_CHAR)
            {
                // Move to the next character then exit the loop
                i++;
                break;
            }
            else if (c == CONFIG_FIELD_DECLARATION_CHAR ||
                     c == CONFIG_STRING_ENCLOSE_CHAR ||
                     c == CONFIG_COMMENT_CHAR ||
                     c == CONFIG_ARRAY_START_CHAR ||
                     c == CONFIG_ARRAY_END_CHAR)
            {
                break;
            }
            else
            {
                string_write_char(dest, c);
            }
        }
    }

    // Null terminate the field we've extracted
    string_null_terminate(dest);

    // Return the position in the string of the next unread char
    *pos = i;
    return TRUE;
}

config_t read_config_file(char *filePath)
{
    // Storing the processed data
    dynamic_array_t results = new_dynamic_array(sizeof(config_var_t));
    dynamic_array_t fields = new_dynamic_array(sizeof(string_t));
    config_var_t newVar;
    config_t config;

    // reading from the file
    FILE *configFile = fopen(filePath, MODE_READ);
    string_t buffer = new_string(DEFAULT_BUFFER_LEN);
    string_t field = new_string(DEFAULT_BUFFER_LEN);

    /*
     * Read every line in the file
    */
    int inArray = FALSE;
    int named = FALSE;
    int currField = CONFIG_FIELD_NAME;
    int complexField = FALSE;
    int currentConfigSaved = TRUE;
    int addedDeclaration = FALSE;

    while (fileio_next_line(configFile, &buffer))
    {
        unsigned int pos = 0;
        while (extract_field(buffer, &pos, &field, &complexField))
        {
            if (!complexField && field.len == 1)
            {
                if (field.str[0] == CONFIG_COMMENT_CHAR)
                {
                    break;
                }
                else if (field.str[0] == CONFIG_FIELD_DECLARATION_CHAR)
                {
                    currField = CONFIG_FIELD_DECLARATION;
                    continue;
                }
                else if (field.str[0] == CONFIG_ARRAY_START_CHAR)
                {
                    inArray = TRUE;
                    continue;
                }
                else if (field.str[0] == CONFIG_ARRAY_END_CHAR)
                {
                    currField = CONFIG_FIELD_NONE;
                    inArray = FALSE;
                }
            }

            if (currField == CONFIG_FIELD_NAME)
            {
                if (!named && field.len != 0)
                {
                    currentConfigSaved = FALSE;
                    newVar.varName = string_copy(field);
                    newVar.data = NULL;
                    newVar.len = 0;

                    // Reset the array for storing the new data
                    fields.len = 0;
                    named = TRUE;
                    addedDeclaration = FALSE;
                }
            }
            else if (currField == CONFIG_FIELD_DECLARATION)
            {
                if (field.len != 0 && ((!inArray && !addedDeclaration) || inArray))
                {
                    string_t fieldCpy = string_copy(field);

                    // Add the current field to the dynamic array
                    dynamic_array_append(&fields, &fieldCpy);
                }
            }
        }

        if (!inArray)
        {
            if (!currentConfigSaved)
            {
                currentConfigSaved = TRUE;

                array_t convertedFields = dynamic_array_to_array(&fields);
                newVar.data = convertedFields.dat;
                newVar.len = convertedFields.len;

                dynamic_array_append(&results, &newVar);
            }
            currField = CONFIG_FIELD_NAME;
            named = FALSE;
        }
    }
    fclose(configFile);

    if (!currentConfigSaved)
    {
        array_t convertedFields = dynamic_array_to_array(&fields);
        newVar.data = convertedFields.dat;
        newVar.len = convertedFields.len;

        dynamic_array_append(&results, &newVar);
    }

    array_t restricted = dynamic_array_to_array(&results);
    array_t final = tree_sort(restricted, sizeof(config_var_t), config_var_compare);

    array_destroy(restricted);

    config.len = final.len;
    config.vars = final.dat;
    config.modified = FALSE;
    config.configLocation = filePath;

    // Dealing with memory
    string_destroy(&buffer);
    string_destroy(&field);

    dynamic_array_destroy(results);
    dynamic_array_destroy(fields);

    return config;
}

void config_encode(string_t *dest, string_t *toEncode)
{
    dest->len = 0;
    // Write a quote-formatted version of the string
    for (int x = 0; x < toEncode->len; x++)
    {
        char c = toEncode->str[x];
        if (c == CONFIG_STRING_ENCLOSE_CHAR)
        {
            string_write_char(dest, CONFIG_ESCAPE_CHAR);
            string_write_char(dest, CONFIG_STRING_ENCLOSE_CHAR);
        }
        else
        {
            string_write_char(dest, c);
        }
    }
    string_null_terminate(dest);
}

/*
 * If any variables in the config have been modified, saves the config file.
 * If you have modified any variables directly, ensure to set the modified flag
 * to TRUE (1) or the config file will not save
 * 
 * returns TRUE if successful, FALSE if file failed to open
*/
int config_save(config_t config)
{
    /*
     * Nothing to save...
    */
    if (!config.modified)
    {
        return TRUE;
    }

    string_t processed = new_string(DEFAULT_BUFFER_LEN);

    FILE *cfgOut = fopen(config.configLocation, MODE_WRITE);

    if (!cfgOut)
    {
        return FALSE;
    }

    /*
     * Write the standard config header comment
    */
    fprintf(cfgOut, CONFIG_STANDARD_DESCRIPTION);
    for (int i = 0; i < config.len; i++)
    {
        config_encode(&processed, &config.vars[i].varName);
        
        fprintf(cfgOut, "\"%s\" = [\n", processed.str);

        if (config.vars[i].len == 0)
        {
            fprintf(cfgOut, "\t%s\n", CONFIG_NO_ENTRY);
        }
        else
        {
            for (int j = 0; j < config.vars[i].len; j++)
            {
                config_encode(&processed, &config.vars[i].data[j]);

                fprintf(cfgOut, "\t\"%s\"\n", processed.str);
            }
        }

        fprintf(cfgOut, "\t]\n\n");
    }

    string_destroy(&processed);
    fclose(cfgOut);

    return TRUE;
}

config_var_t *config_get_var(config_t *config, char *name)
{
    /*
     * Convert the name into a searchable format
    */
    config_var_t key;
    key.len = 1;
    key.varName.str = name;

    /*
     * Search the sorted config list for the requested var
    */
    config_var_t *var = bsearch(&key, config->vars, config->len, sizeof(config_var_t), config_var_compare);

    return var;
}

int config_var_compare(const void *var1, const void *var2)
{
    config_var_t *a = (config_var_t *)var1;
    config_var_t *b = (config_var_t *)var2;

    if (!a || !a->varName.str)
    {
        if (!b || !b->varName.str)
        {
            return 0;
        }
        else
        {
            // A is less than b
            return -1;
        }
    }
    else if (!b || !b->varName.str)
    {
        if (!a || !a->varName.str)
        {
            return 0;
        }
        else
        {
            // b is greather than a
            return 1;
        }
    }

    // Both strings exist, comparing
    return strcmp(a->varName.str, b->varName.str);
}

/*
 * Free's all dynamically allocated data in a Config
*/
void config_destroy(config_t config)
{
    for (int i = 0; i < config.len; i++)
    {
        config_var_t var = config.vars[i];

        string_destroy(&var.varName);
        for (int i = 0; i < var.len; i++)
        {
            string_destroy(&var.data[i]);
        }
        destroy(var.data);
    }
    destroy(config.vars);
}

void config_print_vars(config_t config)
{
    printf("CONFIG Contents:\n-----------------\n");
    for (int i = 0; i < config.len; i++)
    {
        printf("%s\n", config.vars[i].varName.str);
        if (config.vars[i].len == 0)
        {
            printf("\t\"(null)\"\n");
        }
        else
        {
            for (int j = 0; j < config.vars[i].len; j++)
            {
                printf("\t\"%s\"\n", config.vars[i].data[j].str);
            }
        }
        printf("\n");
    }
    printf("-----------------\n");
}